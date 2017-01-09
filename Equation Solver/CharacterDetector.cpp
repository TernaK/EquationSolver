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
  
  //get the rois in sorted order
  vector<Rect> charRois;
  
  //for each roi, get the character
  for(auto charRoi: charRois){
    characters.push_back(ocr.detectLetter(image(charRoi)));
  }
}


void getCharRois(const cv::Mat& image, std::vector<cv::Rect>& rois){
  //preprocess image
  
  //get largest objects/size distribution
  
  //pick out rois
  
  //sort rois based on size
}