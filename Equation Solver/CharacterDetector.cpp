//
//  CharacterDetector.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "CharacterDetector.hpp"
using namespace cv;
using namespace std;


CharactedDetector::CharactedDetector(std::string modelFile)
: ocr(modelFile) {
}


void CharactedDetector::detectCharacters(const cv::Mat& image, std::vector<char> characters){
  Mat equationImage;
  bool equationFound = getEquationImage(image, equationImage);
  
  if(equationFound){
    //get character rois
    vector<Rect> charRois;
    getCharRois(equationImage, charRois);
  }
  else {
    cout << "Could not find equation" << endl;
  }
  
//  //for each roi, get the character
//  for(auto charRoi: charRois){
//    characters.push_back(ocr.detectLetter(image(charRoi)));
//  }
}

bool CharactedDetector::getEquationImage(const cv::Mat& image, Mat& equationImage){
  
  //clone
  Mat grayImage = image.clone();
  
  GaussianBlur(image, grayImage, Size(3,3), 0.8);
  
  // perform canny
  Mat canny;
  Canny(grayImage, canny, 80, 150);
  
  //get contours
  vector<vector<Point> > contours;
  findContours(canny, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
  
  
  //filter out bad contours
  vector<vector<Point> > approxContours;
  for(auto contour: contours){
    //approximate
    vector<Point> newContour;
    approxPolyDP(contour, newContour, 10, true);
    if(isContourConvex(newContour) && newContour.size() == 4)
      approxContours.push_back(newContour);
  }
  
  if(approxContours.size() < 1)
    return false;
  
  Rect eqRect;
  getMostProbableRectFromContours(approxContours, image.cols, image.rows, eqRect);
  
  if(eqRect.width > 0){
    equationImage = grayImage(eqRect).clone();
    return true;
  }
  
  return false;
  
}

bool compareRectXPos ( cv::Rect rect1, cv::Rect rect2 ) {
  return rect1.x < rect2.x;
}

void CharactedDetector::getMostProbableRectFromContours(const std::vector<std::vector<cv::Point> > contours, int width, int height, cv::Rect& roi){
  
  int border = 10;
  
  int maxIndex = 0;
  int maxArea = 0;
  for(int i = 0; i < contours.size(); i++){
    double area = fabs( contourArea(cv::Mat(contours[i])) );
    if(area > maxArea){
      maxArea = area;
      maxIndex = i;
    }
  }
  
  roi = Rect(contours[maxIndex][0], contours[maxIndex][2]);
  roi.x += border;
  roi.y += border;
  roi.width -= border*2;
  roi.height -= border*2;
}

//left,top,width,height,area
void CharactedDetector::getCharRois(cv::Mat& image, std::vector<cv::Rect>& rois){
  //preprocess image
  int width = 200;
  int height = float(image.rows)/float(image.cols) * width;
  //resize
  resize(image, image, Size(width, height));
  //binarize
  adaptiveThreshold(image, image, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 21, 5);
  //close
  Mat morphElement = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3,3));
  morphologyEx(image, image, MorphTypes::MORPH_CLOSE, morphElement);

  //get connected components
  Mat labels, stats, centroids;
  int numObjects = connectedComponentsWithStats(image, labels, stats, centroids);
  
  //get components stats (area)
  Mat areaMat = stats.col(4);
  double min, max;
  Point minLoc, maxLoc;
  minMaxLoc(areaMat, &min, &max, &minLoc, &maxLoc);
  
  //threshold to discard objects
  float avgArea = (sum(areaMat)[0] - areaMat.at<int>(maxLoc)) / (areaMat.rows - 1);
  //half the average area is the threshold
  for(int i = 0; i < areaMat.rows; i++){
    //ignore the max area componnet, i.e. the background
    if(i != maxLoc.y && areaMat.at<int>(Point(0,i)) > (avgArea/2) ){
      Rect roi(stats.at<int>(Point(0,i)),
               stats.at<int>(Point(1,i)),
               stats.at<int>(Point(2,i)),
               stats.at<int>(Point(3,i)) );
      rois.push_back(roi);
      rectangle(image, roi, Scalar(150));
    }
  }
  
  sort(rois.begin(), rois.end(), compareRectXPos);
  
  namedWindow("equation");
  imshow("equation", image);
  waitKey();
}
