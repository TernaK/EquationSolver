//
//  Ocr.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "Ocr.hpp"
using namespace cv;
using namespace std;

Ocr::Ocr(std::string modelFile){
  ModelLoader::loadModel(modelFile, model);
}

char Ocr::detectLetter(const cv::Mat& image){
  Mat x, y;
  int opID = model.feedForward(x, y);
  return dictionary[opID];
}