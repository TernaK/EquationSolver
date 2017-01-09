//
//  CharacterDetector.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "CharacterDetector.hpp"


CharactedDetector::CharactedDetector(std::string modelFile)
: ocr(modelFile) {
}


void CharactedDetector::detectCharacters(const cv::Mat& image, std::vector<char> characters){
  
}