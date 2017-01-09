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
    namedWindow("equation");
    imshow("equation", equationImage);
    waitKey();
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
  
  //get grayscale
  Mat grayImage;
  if(image.channels() != 1)
    cvtColor(image, grayImage, CV_BGR2GRAY);
  else
    grayImage = image.clone();
  
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
    equationImage = image(eqRect);
    return true;
  }
  
  return false;
  
  //draw
//  drawContours(image, approxContours, -1, Scalar(0,255,0), 2);
  //Mat imageClone = image.clone();
  //rectangle(imageClone, eqRect, Scalar(0,255,0));
  
//  cout << contours.size() << endl;
  
//  namedWindow("contours");
//  imshow("contours", imageClone);
//  waitKey();
  
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
  /*
  
  int dim = 100;
  
  Mat centerMap = Mat::zeros(dim, dim, CV_8U);
  
  //map contour centers, height, and width to points in image
  Mat widthMap = Mat::zeros(1, dim, CV_8U);
  Mat heightMap = Mat::zeros(1, dim, CV_8U);
  
  for(auto contour: contours){
    Point center(0,0);
    for(auto vertex: contour){
      center += vertex;
    }
    center = center/(float)contour.size();
    center = Point(float(center.x*dim)/width, float(center.y*dim)/height);
    centerMap.at<char>(center) = 255;
    
    //width
    float dx = abs(contour[0].x - contour[1].x)*dim/width;
    widthMap.at<char>(Point(dx,0)) = 255;
    
    //height
    float dy = abs(contour[1].y - contour[2].y)*dim/height;
    heightMap.at<char>(Point(dy,0)) = 255;
  }
  
  //values
  float cx,cy,xLen,yLen;
  
  //get max response
  Mat filtered;
  boxFilter(centerMap, filtered, -1, Size(5,5));
  double minVal, maxVal;
  Point minLoc, maxLoc;
  minMaxLoc(filtered, &minVal, &maxVal, &minLoc, &maxLoc);
  
  cx = maxLoc.x * width / dim;
  cy = maxLoc.y * height / dim;
  
  boxFilter(widthMap, filtered, -1, Size(5,1));
  minMaxLoc(filtered, &minVal, &maxVal, &minLoc, &maxLoc);
  xLen = maxLoc.x * width / dim;
  
  boxFilter(heightMap, filtered, -1, Size(5,1));
  minMaxLoc(filtered, &minVal, &maxVal, &minLoc, &maxLoc);
  yLen = maxLoc.x * height / dim;
  
  roi = Rect(cx-xLen/2, cy-yLen/2, xLen, yLen);
  */
}


void getCharRois(const cv::Mat& image, std::vector<cv::Rect>& rois){
  //preprocess image
  
  //get largest objects/size distribution
  
  //pick out rois
  
  //sort rois based on size
}
