//
//  DataExtractor.cpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/19/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "DataExtractor.hpp"
using namespace std;
using namespace cv;

DataExtractor::DataExtractor(std::string imageFileName, std::string outputFileName, int rows, int cols):
	imageFileName(imageFileName),
	outputFileName(outputFileName),
	rows(rows),
	cols(cols) {
}

/*
 * get the bounding box of the data
 *
 **/
bool DataExtractor::getBounds(const cv::Mat& dataImage, std::vector<cv::Point>& bounds){
  
  //TODO: maybe have to process the image to avoid small contours messing this up
  
  //find contours
  vector<vector<Point>> originalContours;
  findContours(dataImage, originalContours, RETR_LIST, CHAIN_APPROX_SIMPLE);
  
  //appriximate contours
  vector<vector<Point>> approxContours;
  approxPolyDP(originalContours, approxContours, 10, true);
  
  //select only 4 sided ones
  vector<vector<Point>> finalContours;
  for(auto contour: approxContours){
    if(contour.size() == 4)
      finalContours.push_back(contour);
  }
  
  //return if number of contours left is 1
  if(finalContours.size() == 1){
    bounds = finalContours.front();
  }
  return finalContours.size() == 1;
}


bool DataExtractor::getTransformedBoundsImage(const Mat& dataImage, const vector<Point>& vertices, Mat& transformedImage){
  int width = MAP_SQUARE_SIZE * cols;
  int height = MAP_SQUARE_SIZE * rows;
  vector< Point2f> mappedPointsOutput = {
    Point2f(0,height),
    Point2f(width,height),
    Point2f(width,0),
    Point2f(0,0),
  };
  
  //convert Point<int> to Point<float>: the input mapping
  vector<Point2f> floatContour = {
    Point2f(vertices[0].x, vertices[0].y),
    Point2f(vertices[1].x, vertices[1].y),
    Point2f(vertices[2].x, vertices[2].y),
    Point2f(vertices[3].x, vertices[3].y),
  };
  
  //perform perspective transformation
  Mat transform = getPerspectiveTransform(floatContour, mappedPointsOutput);
  warpPerspective(dataImage, transformedImage, transform, Size(width,height));
  return true;
}

bool DataExtractor::getImages(std::vector<cv::Mat>& samples){
  
  //get gray
  Mat image, gray;
  if(image.channels() > 1)
    cvtColor(image, gray, CV_RGBA2GRAY);
  else
    gray = image.clone();
  
  //get bounds
  vector<Point> bounds;
  getBounds(gray, bounds);
  
  //get data image
  Mat dataImage;
  getTransformedBoundsImage(gray, bounds, dataImage);
  
  namedWindow("extracted");
  imshow("extracted", dataImage);
  waitKey();
  
  return true;
}


bool DataExtractor::writeToFoldersAndFiles(const std::vector<cv::Mat>& samples, std::string imageData){
  return true;
}