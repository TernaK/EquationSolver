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
    int width = 200;
    int height = float(equationImage.rows)/float(equationImage.cols) * width;
    
    resize(equationImage, equationImage, Size(width, height));
    
    adaptiveThreshold(equationImage, equationImage, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 21, 5);
    
    Mat morphElement = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3,3));
    morphologyEx(equationImage, equationImage, MorphTypes::MORPH_CLOSE, morphElement);
  
    namedWindow("equation");
    imshow("equation", equationImage);
    waitKey();
    
    imwrite("/Users/Terna/Desktop/binary.png", equationImage);
  }
  else {
    cout << "Could not find equation" << endl;
  }
//  //get the rois in sorted order
//  vector<Rect> charRois;
//  
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

bool compareContourAreasDesc ( std::vector<cv::Point> contour1, std::vector<cv::Point> contour2 ) {
  double i = fabs( contourArea(cv::Mat(contour1)) );
  double j = fabs( contourArea(cv::Mat(contour2)) );
  return ( i > j );
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


void getCharRois(const cv::Mat& image, std::vector<cv::Rect>& rois){
  //preprocess image
  
  //get largest objects/size distribution
  
  //pick out rois
  
  //sort rois based on size
}
