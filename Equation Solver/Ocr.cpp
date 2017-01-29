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

Ocr::Ocr(std::string protoFile, std::string modelFile){
  
  //! [Create the importer of Caffe model]
  Ptr<dnn::Importer> importer;
  try                                     //Try to import Caffe GoogleNet model
  {
    importer = dnn::createCaffeImporter(protoFile, modelFile);
  }
  catch (const cv::Exception &err)        //Importer can throw errors, we will catch them
  {
    std::cerr << err.msg << std::endl;
  }
  //! [Create the importer of Caffe model]
  
  if (!importer)
  {
    std::cerr << "Can't load network by using the following files: " << std::endl;
    std::cerr << "prototxt:   " << protoFile << std::endl;
    std::cerr << "caffemodel: " << modelFile << std::endl;
    exit(-1);
  }
  
  //! [Initialize network]
  importer->populateNet(net);
  importer.release();                     //We don't need importer anymore
  //! [Initialize network]
  
  
  loadDictionary();
  
}


cv::Mat Ocr::preprocessImage(const cv::Mat& letterImage){
  
  //first refit the image
  //the old dimensions
  float _width = letterImage.cols;
  float _height = letterImage.rows;
  
  //the new dimensions
  int width;
  int height;
  
  //use the larger dimension to resize
  if(_width > _height){
    width = INNER_DIM;
    height = round(_height/_width * INNER_DIM);
  }
  else{
    height = INNER_DIM;
    width = round(_width/_height * INNER_DIM);
  }
  
  //resize
  Mat inner;
  resize(letterImage, inner, Size(width, height));
  
  //get the offsets
  int x = (OUTER_DIM-width)/2;
  int y = (OUTER_DIM-height)/2;
  
  //copy to roi
  Mat outer = Mat::zeros(OUTER_DIM, OUTER_DIM, CV_8U);
  inner.copyTo(outer(Rect(x,y,width,height)));
  
  Mat binary;
  adaptiveThreshold(outer, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 17, 10);
  
  return binary;
}

char Ocr::detectLetter(const cv::Mat& image, bool preprocess){
  //! [Prepare blob]
  Mat img = image.clone();
  
  if(preprocess)
  	img = preprocessImage(img);
  return 0;
  
  img.convertTo(img, CV_32F);
  img /= 255.0f;
  //resize(img, img, Size(28, 28));
  dnn::Blob inputBlob = dnn::Blob::fromImages(img);   //Convert Mat to dnn::Blob image batch
  
  //! [Set input blob]
  net.setBlob(".data", inputBlob);        //set the network input
  //! [Set input blob]
  
  //! [Make forward pass]
  net.forward();                          //compute output
  //! [Make forward pass]
  
  //! [Gather output]
  dnn::Blob prob = net.getBlob("prob");   //gather output of "prob" layer
  
  int classId;
  double classProb;
  getMaxClass(prob, &classId, &classProb);//find the best class
  //! [Gather output]
  
  //cout << img << endl << "Class #" << classId << endl;
  
  return dictionary[classId];
}

void Ocr::getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
  Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
  Point classNumber;
  std::cout << probMat << endl;
  minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
  *classId = classNumber.x;
}

void Ocr::loadDictionary(){
  char c = '0';
  for(int i = 0; i < 10; i++){
    dictionary[i] = c;
  }
  dictionary[10] = '+';
  dictionary[11] = '-';
  dictionary[12] = '/';
  dictionary[13] = '*';
}
