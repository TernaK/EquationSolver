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

DataExtractor::DataExtractor(std::string imageFileName, std::string outputFileName, int rows, int cols, int border):
	imageFileName(imageFileName),
	outputFileName(outputFileName),
	rows(rows),
	cols(cols),
	border(border){
  image = imread(imageFileName);
}


DataExtractor::DataExtractor(const cv::Mat& image, std::string outputFileName, int rows, int cols, int border):
	image(image),
	outputFileName(outputFileName),
	rows(rows),
	cols(cols),
	border(border){
}

/*
 * get the bounding box of the data
 *
 **/
bool DataExtractor::getBounds(const cv::Mat& dataImage, std::vector<cv::Point>& bounds){
  
  //TODO: maybe have to process the image to avoid small contours messing this up
  Mat gauss, canny;
  
  GaussianBlur(dataImage, gauss, Size(3,3), 0.8);
//  adaptiveThreshold(gauss, gauss, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 5);
  Canny(gauss, canny, 80, 150);
  
  //find contours
  vector<vector<Point>> originalContours;
  findContours(canny, originalContours, RETR_LIST, CHAIN_APPROX_SIMPLE);
  
  //appriximate contours
  vector<vector<Point>> approxContours;
  for(auto contour: originalContours){
    vector<Point> approxContour;
    approxPolyDP(contour, approxContour, 10, true);
    approxContours.push_back(approxContour);
  }
  
  //select only 4 sided ones
  vector<vector<Point>> finalContours;
  for(auto contour: approxContours){
    if(contour.size() == 4)
      finalContours.push_back(contour);
  }
  
  if(finalContours.size() < 1)
    return false;
  
  Mat x = dataImage.clone();
  drawContours(x, approxContours, -1, Scalar(255,0,255));
  cvtColor(x, x, CV_GRAY2RGB);
  
//  namedWindow("extracted");
//  imshow("extracted", x);
//  waitKey();
  
  
  getMostProbableVerticesFromContours(finalContours, bounds);
  
  return false;
}

void DataExtractor::getMostProbableVerticesFromContours(const std::vector<std::vector<cv::Point> > contours, std::vector<cv::Point>& vertices){
  
  int maxIndex = 0;
  int maxArea = 0;
  for(int i = 0; i < contours.size(); i++){
    double area = fabs( contourArea(cv::Mat(contours[i])) );
    if(area > maxArea){
      maxArea = area;
      maxIndex = i;
    }
  }
  
  vertices = contours[maxIndex];
}


bool DataExtractor::getTransformedBoundsImage(const Mat& dataImage, const vector<Point>& vertices, Mat& transformedImage){
  int width = MAP_SQUARE_SIZE * cols;
  int height = MAP_SQUARE_SIZE * rows;
  vector< Point2f> mappedPointsOutput = {
    Point2f(0,0),
    Point2f(0,height),
    Point2f(width,height),
    Point2f(width,0),
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

bool DataExtractor::getImagesCommon(const cv::Mat& gray, std::vector<cv::Mat>& samples, const std::vector<cv::Point>& bounds){
  
  //get data image
  Mat dataImage;
  getTransformedBoundsImage(gray, bounds, dataImage);
  
//    namedWindow("extracted");
//    imshow("extracted", dataImage);
//    waitKey();
  
  for(int i = border ; i < rows-border; i++){
    int row = i * MAP_SQUARE_SIZE;
    for(int j = border; j < cols-border; j++){
      int col = j * MAP_SQUARE_SIZE;
      samples.push_back(dataImage(Rect(Point(col,row), Point(col+MAP_SQUARE_SIZE,row+MAP_SQUARE_SIZE))));
    }
  }
  
  return true;
}

bool DataExtractor::getImages(std::vector<cv::Mat>& samples){
  
  //get gray
  Mat gray;
  if(image.channels() > 1)
    cvtColor(image, gray, CV_RGBA2GRAY);
  else
    gray = image.clone();

  //get bounds
  vector<Point> bounds;
  getBounds(gray, bounds);

  return getImagesCommon(gray, samples, bounds);
}


bool DataExtractor::getImages(std::vector<cv::Mat>& samples, const std::vector<cv::Point>& bounds){
  
  //get gray
  Mat gray;
  if(image.channels() > 1)
    cvtColor(image, gray, CV_RGBA2GRAY);
  else
    gray = image.clone();
  
  return getImagesCommon(gray, samples, bounds);
}


bool DataExtractor::writeToFoldersAndFiles(const std::vector<cv::Mat>& samples, std::string imageData){
  return true;
}

void DataExtractor::refitSample(const cv::Mat& image, const cv::Mat binaryImage, cv::Mat& refittedImage, int innerDim, int outerDim, bool divideSymbol){
  
  //get roi
  //label binary
  //close
//  Mat closed;
//  Mat morphElement = getStructuringElement(MorphShapes::MORPH_ELLIPSE, Size(3,3));
//  morphologyEx(binaryImage, closed, MorphTypes::MORPH_CLOSE, morphElement);
  
//  imshow("im", closed);
//  waitKey();
  
  //get connected components
  Mat labels, stats, centroids;
  connectedComponentsWithStats(binaryImage, labels, stats, centroids);
  
  //largest area (background)
  int max = 0;
  int maxIdx = 0;
  for(int i = 1; i < stats.rows; i++){
    int area = stats.at<int>(Point(CC_STAT_AREA,i));
    if(area > max){
      max = area;
      maxIdx = i;
    }
  }
  
  Rect roi(stats.at<int>(Point(0,maxIdx)),
           stats.at<int>(Point(1,maxIdx)),
           stats.at<int>(Point(2,maxIdx)),
           stats.at<int>(Point(3,maxIdx)) );
  
  //find larger dimension
  float _width = roi.width;
  float _height = roi.height;
  int width, height;
  
  //set dimensions
  if(_width > _height){ //width
    height = round(_height/_width * innerDim);
    width = innerDim;
  }
  else{
    width = round(_width/_height * innerDim);
    height = innerDim;
  }
  
  //offset
  int x = (outerDim - width)/2;
  int y = (outerDim - height)/2;
  
  //resize
  Mat inner = image(roi);
  
  
//  imshow("test", inner);
//  waitKey();
  resize(inner, inner, Size(width, height));
  
  Mat binary;
  adaptiveThreshold(inner, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 17, 10);
  
  //place inside
  refittedImage = Mat::zeros(outerDim, outerDim, CV_8U);
  binary.copyTo(refittedImage(Rect(x,y,width,height)));
  
}