//
//  Ocr.hpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef Ocr_hpp
#define Ocr_hpp

#include "EquationSolver.hpp"
#include "Model.hpp"
#include "ModelLoader.hpp"

class Ocr {
private:
  Model model;
  void loadModel(std::string modelFile);
  cv::Mat preprocessImage(const cv::Mat& letterImage);
  char predictLetter(const cv::Mat& preprocessed);
  
public:
  Ocr(std::string modelFile);
  char detectLetter(const cv::Mat& image);
};

#endif /* Ocr_hpp */
