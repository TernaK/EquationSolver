//
//  CharacterDetector.hpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef CharacterDetector_hpp
#define CharacterDetector_hpp

#include "EquationSolver.hpp"
#include "Ocr.hpp"

/* This class is used to detect characters in an image
 * The image is a rectanglar image containing the characters
 */
class CharactedDetector {
private:
  Ocr ocr; //used to predict a character given it's roi image
  
  /* Obtain the rois of all characters present in the image
   * image: the image containing the characters
   * rois:output the rois of the characters
   */
  void getCharRois(cv::Mat& image, std::vector<cv::Rect>& rois);
  
  /* Given an image, get the rectangle containing the expression
   * image: the image containing the equation
   * equationImage:output the rectangle containing the expression
   */
  bool getEquationImage(const cv::Mat& image, cv::Mat& equationImage);
  
  //TODO: Use the transformed image here, don't assume the rect is ideal
  /* Given a set of contours, find the one most likely to contain the expression
   * roi:output the rect
   */
  void getMostProbableRectFromContours(const std::vector<std::vector<cv::Point>> contours, cv::Rect& roi);
  
public:
  CharactedDetector(std::string modelFile);
  
  /* Detect characters in an image
   * image: the image containing the characters
   * characters:output the characters detected in the image
   */
  void detectCharacters(const cv::Mat& image, std::vector<char> characters);
};

#endif /* CharacterDetector_hpp */
