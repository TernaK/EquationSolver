//
//  MnistReader.cpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/24/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "MnistReader.hpp"
using namespace std;
using namespace cv;

bool MnistReader::readData(std::string dataFile, std::string labelFile, std::vector<std::pair<cv::Mat,int>>& samples){
  
  ifstream dataStream(dataFile);
  if(!dataStream.is_open())
    return false;
  
  ifstream labelStream(labelFile);
  if(!labelStream.is_open())
    return false;
  
  int numSamples;
  uint32_t dataInt;
  uint8_t dataByte;
  
  //data stream
  dataInt = 0; //magic number
  for(int i = 0; i < 4; i++) dataStream >> dataByte; //discard int
  
  numSamples = 0; //number of images
  for(int i = 0; i < 4; i++){ //read int
    dataStream >> dataByte;
    numSamples <<= 8;
    numSamples |= dataByte;
  }
  
  //height
  for(int i = 0; i < 4; i++) dataStream >> dataByte; //discard int
  //cols
  for(int i = 0; i < 4; i++) dataStream >> dataByte; //discard int
  
  //label stream
  dataInt = 0; //magic number
  for(int i = 0; i < 4; i++) labelStream >> dataByte; //discard int
  
  numSamples = 0; //number of images
  for(int i = 0; i < 4; i++){ //read int
    labelStream >> dataByte;
    numSamples <<= 8;
    numSamples |= dataByte;
  }
  
  int N = 28;
  uint8_t data[N*N];
  
  for(int i = 0; i < numSamples; i++){
    std::pair<cv::Mat,int> sample;
    
    //data
    for(int j = 0; j < N*N; j++){
      dataStream >> dataByte;
      data[j] = dataByte;
    }
    sample.first = Mat(N,N,CV_8U,data);
    
    //label
    labelStream >> dataByte;
    sample.second = dataByte;
    
    imshow("data", sample.first);
    waitKey(30);
    
    samples.push_back(sample);
  }
    
  dataStream.close();
  labelStream.close();
  return true;
}
