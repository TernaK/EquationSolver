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
#include <opencv2/dnn.hpp>


class Ocr {
private:
  void getMaxClass(cv::dnn::Blob &probBlob, int *classId, double *classProb);
  cv::Mat preprocessImage(const cv::Mat& letterImage);
  char predictLetter(const cv::Mat& preprocessed);
  void loadDictionary();
  
  cv::dnn::Net net;
  std::map<int, char> dictionary;
  
public:
  Ocr(){};
  Ocr(std::string protoFile, std::string modelFile);
  char detectLetter(const cv::Mat& image, bool preprocess=true);
};

#endif /* Ocr_hpp */
