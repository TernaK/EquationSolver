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
  dnn::Net net;
  importer->populateNet(net);
  importer.release();                     //We don't need importer anymore
  //! [Initialize network]
  
}

char Ocr::detectLetter(const cv::Mat& image){
  //! [Prepare blob]
  Mat img = image.clone();
  
  img.convertTo(img, CV_32F);
  img /= 255.0f;
  //resize(img, img, Size(28, 28));       //GoogLeNet accepts only 224x224 RGB-images
  dnn::Blob inputBlob;// = dnn::Blob::fromImages(img);   //Convert Mat to dnn::Blob image batch
  //! [Prepare blob]
  
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
  
  return 0;
}

void Ocr::getMaxClass(dnn::Blob &probBlob, int *classId, double *classProb)
{
  Mat probMat = probBlob.matRefConst().reshape(1, 1); //reshape the blob to 1x1000 matrix
  Point classNumber;
  std::cout << probMat << endl;
  minMaxLoc(probMat, NULL, classProb, NULL, &classNumber);
  *classId = classNumber.x;
}
