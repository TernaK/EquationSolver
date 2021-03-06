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


CharactedDetector::CharactedDetector(std::string protoFile, std::string modelFile){
  ocr = Ocr(protoFile, modelFile);
}


void CharactedDetector::detectCharacters(const cv::Mat& image, std::vector<char>& characters){
  Mat equationImage;
  bool equationFound = getEquationImage(image, equationImage);
  
  vector<Rect> charRois;
  if(equationFound){
    //get character rois
    getCharRois(equationImage, charRois);
  }
  else {
    cout << "Could not find equation" << endl;
  }
  
  //for each roi, get the character
  for(auto charRoi: charRois){
//    imshow("img", equationImage(charRoi)); waitKey();
    characters.push_back(ocr.detectLetter(equationImage(charRoi)));
  }
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
  getMostProbableRectFromContours(approxContours, eqRect);
  
  if(eqRect.width > 0){
    equationImage = image(eqRect).clone();
    return true;
  }
  
  return false;
  
}


void CharactedDetector::getMostProbableRectFromContours(const std::vector<std::vector<cv::Point> > contours, cv::Rect& roi){
  
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

bool compareRectXPos ( cv::Rect rect1, cv::Rect rect2 ) {
  return rect1.x < rect2.x;
}

//left,top,width,height,area
void CharactedDetector::getCharRois(cv::Mat& image, std::vector<cv::Rect>& rois){
  //preprocess image
  int width = 200;
  int height = float(image.rows)/float(image.cols) * width;
  //resize
  resize(image, image, Size(width, height));
  //binarize
  Mat binary;
  adaptiveThreshold(image, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 21, 5);
  //close
  Mat morphElement = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3,3));
  morphologyEx(binary, binary, MorphTypes::MORPH_CLOSE, morphElement);
  
  //get connected components
  Mat labels, stats, centroids;
  int numObjects = connectedComponentsWithStats(binary, labels, stats, centroids);
  
  //need at least two operands & one operation
  if(numObjects < 3)
    return;
  
  //get components stats (area)
  Mat areaMat = stats.col(CC_STAT_AREA);
  double min, max;
  Point minLoc, maxLoc;
  minMaxLoc(areaMat, &min, &max, &minLoc, &maxLoc);
  
  //threshold to discard objects
  float avgArea = (sum(areaMat)[0] - areaMat.at<int>(maxLoc)) / (areaMat.rows - 1);
  //half the average area is the threshold
  for(int i = 0; i < areaMat.rows; i++){
    int midY = stats.at<int>(Point(CC_STAT_TOP,i)) + stats.at<int>(Point(CC_STAT_HEIGHT,i))/2;
    bool isInMidThird = (midY > height/3) && (midY < (height*2)/3);
    //ignore the max area componnet, i.e. the background
    if(i != maxLoc.y && areaMat.at<int>(Point(0,i)) > (avgArea/2) && isInMidThird){
      Rect roi(stats.at<int>(Point(CC_STAT_LEFT,i)),
               stats.at<int>(Point(CC_STAT_TOP,i)),
               stats.at<int>(Point(CC_STAT_WIDTH,i)),
               stats.at<int>(Point(CC_STAT_HEIGHT,i)) );
      rois.push_back(roi);
      rectangle(binary, roi, Scalar(150));
    }
  }
  
  sort(rois.begin(), rois.end(), compareRectXPos);
  
  //namedWindow("equation");
  //imshow("equation", binary);
  //waitKey();
}
