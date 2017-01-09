//
//  Ocr.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "Ocr.hpp"

Ocr::Ocr(std::string modelFile){
  ModelLoader::loadModel(modelFile, model);
}

char Ocr::detectLetter(const cv::Mat& image){
  return 0;
}