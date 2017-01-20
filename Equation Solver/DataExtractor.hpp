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

#define MAP_SQUARE_SIZE 30

class DataExtractor {
private:
  std::string imageFileName;//source file name of the samples image
  std::string outputFileName;//where to store the images
  int rows; //number or rows of samples
  int cols; //number of columns of samples
  cv::Mat image;
  
  
  /* get the vertices of the bounding box drawn in the image
   * dataImage: the image containing the samples
   * bounds output: vertices of the bounds, array of Points
   */
  bool getBounds(const cv::Mat& dataImage, std::vector<cv::Point>& bounds);
  
  /* obtain  a rectangle image of the written samples
   * dataImage: the image containing the samples
   * vertices: the vertices of the bounding box in the image
   * transformedImage output: the rectangle image
   */
  bool getTransformedBoundsImage(const cv::Mat& dataImage, const std::vector<cv::Point>& vertices, cv::Mat& transformedImage);
  
public:
  /* set parameters for the detection and extraction
   * ...: see private members
   */
  DataExtractor(std::string imageFileName, std::string outputFileName, int rows, int cols);
  
  /* obtain an array of individual image samples
   * samples output: images of the samples
   */
  bool getImages(std::vector<cv::Mat>& samples);
  
  /* write the samples to output folders and files
   * samples: the samples images
   * imageData: TODO
   */
  bool writeToFoldersAndFiles(const std::vector<cv::Mat>& samples, std::string imageData);
};



#endif /* DataExtractor_hpp */
