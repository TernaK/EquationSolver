//
//  DataExtractor.hpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/19/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef DataExtractor_hpp
#define DataExtractor_hpp

#include "EquationSolver.hpp"

#define MAP_SQUARE_SIZE 100

/* This class may be used to quickly extract training data from a grid arrangement
 * on paper.
 */
class DataExtractor {
private:
  std::string imageFileName;//source file name of the samples image
  std::string outputFileName;//where to store the images
  int rows; //number or rows of samples
  int cols; //number of columns of samples
  int border; //box border of data on paper
  cv::Mat image; //the image from which to extract data
  
  //see getImages()
  bool getImagesCommon(const cv::Mat& gray, std::vector<cv::Mat>& samples, const std::vector<cv::Point>& bounds);
  
  /* get the vertices of the bounding box drawn in the image
   * dataImage: the image containing the samples
   * bounds:output vertices of the bounds, array of Points
   */
  bool getBounds(const cv::Mat& dataImage, std::vector<cv::Point>& bounds);
  
  /* obtain  a rectangle image of the written samples
   * dataImage: the image containing the samples
   * vertices: the vertices of the bounding box in the image
   * transformedImage:output the rectangle image
   */
  bool getTransformedBoundsImage(const cv::Mat& dataImage, const std::vector<cv::Point>& vertices, cv::Mat& transformedImage);
  
  /* Get the bounding points of the grid
   * contours: the detected contours in the image
   * vertices:output the bounding points of the surrounding quad
   */
  void getMostProbableVerticesFromContours(const std::vector<std::vector<cv::Point> > contours, std::vector<cv::Point>& vertices);
  
  
public:
  /* set parameters for the detection and extraction
   * ...: see private members
   */
  DataExtractor(std::string imageFileName, std::string outputFileName, int rows, int cols, int border=1);
  DataExtractor(const cv::Mat& image, std::string outputFileName, int rows, int cols, int border=1);
  
  /* obtain an array of individual image samples
   * samples:output images of the samples
   */
  bool getImages(std::vector<cv::Mat>& samples);
  bool getImages(std::vector<cv::Mat>& samples, const std::vector<cv::Point>& bounds);
  
  /* write the samples to output folders and files
   * samples: the samples images
   * imageData: TODO
   */
  bool writeToFoldersAndFiles(const std::vector<cv::Mat>& samples, std::string imageData);
};



#endif /* DataExtractor_hpp */
