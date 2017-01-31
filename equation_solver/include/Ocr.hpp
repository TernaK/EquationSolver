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
  
  /* prepare an acquired image for classification
   * letterImage: image containing a letter
   */
  cv::Mat preprocessImage(const cv::Mat& letterImage);
  
  /* return a prediction of what the image is
   * preprocessed: Mat image that has been prepared for classification, see preprocess()
   */
  char predictLetter(const cv::Mat& preprocessed);


  /* prepare the dictionary
   */ 
  void loadDictionary();
  
  cv::dnn::Net net; //the neural network
  std::map<int, char> dictionary; // key, char pairs representing the dictionary
  
public:
  Ocr(){};

  /* set up an Ocr object to use desired caffe files
   * protoFile: the .prototxt file of the caffe model
   * modelFile: the snapshot of a trained caffe model
   */
  Ocr(std::string protoFile, std::string modelFile);

  /* detect what letter is present in an image
   * image: the acquired image
   * preprocess: enable bypassing the preprocessing step (good for debugging)
   */
  char detectLetter(const cv::Mat& image, bool preprocess=true);
};

#endif /* Ocr_hpp */
