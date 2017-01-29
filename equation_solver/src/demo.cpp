//
//  main.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/7/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//
#include "CharacterDetector.hpp"
#include "MathEngine.hpp"
#include "DataExtractor.hpp"
#include "MnistReader.hpp"
#include "Ocr.hpp"
using namespace std;
using namespace cv;

void testMathEnginEvaluate(){

  MathEngine engine;
  float res = engine.evaluate("5*7");
  cout << "parsed expression: " << engine.getParsedString() << endl;
  cout  << "parsed infix expression: " << engine.getInfixString() << endl;
  cout << "result: " << res << "  expected: " << 5.0f*7 << endl << endl;
  
  res = engine.evaluate("0-3*2");
  cout << "parsed expression: " << engine.getParsedString() << endl;
  cout  << "parsed infix expression: " << engine.getInfixString() << endl;
  cout << "result: " << res << "  expected: " << 0-3*2 << endl << endl;
  
  res = engine.evaluate("34*7/20-4");
  cout << "parsed expression: " << engine.getParsedString() << endl;
  cout << "parsed infix expression: " << engine.getInfixString() << endl;
  cout << "result: " << res << "  expected: " << 34.0f*7/20-4 << endl << endl;
  
  res = engine.evaluate("5*8/20-4*2*6*7+5*5");
  cout << "parsed expression: " << engine.getParsedString() << endl;
  cout << "parsed infix expression: " << engine.getInfixString() << endl;
  cout << "result: " << res << "  expected: " << 5.0f*8/20-4*2*6*7+5*5 << endl << endl;
}

void testCharactedDetector(){
  Mat image = imread("data/test3.png", IMREAD_GRAYSCALE);
  
  CharactedDetector detector("dummy", "dummy");
  vector<char> chars;
  detector.detectCharacters(image, chars);
}

void testDataExtractor(string fileName){
  Mat image = imread(fileName, IMREAD_GRAYSCALE);
  resize(image, image, image.size()/6);
  DataExtractor de = DataExtractor(image, "", 15, 15, 1);
  vector<Point> bounds = {Point(149,120), Point(439,112), Point(445,348), Point(159,361)};
  vector<Mat> samples;
  de.getImages(samples);
  
  for(auto sample: samples){
    
    //get binary
    Mat binary;
    adaptiveThreshold(sample, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 15);
    
    //refit
    DataExtractor::refitSample(sample, binary, sample);
    
    //binarize once more
//    adaptiveThreshold(sample, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 15);
    
    
    //namedWindow("extracted");
    //imshow("extracted", binary) ;
    //waitKey();
  }
}

void runDataExtractor(string fileName, string savePrefix){
  
  Mat image = imread(fileName, IMREAD_GRAYSCALE);
  resize(image, image, image.size()/6);
  DataExtractor de = DataExtractor(image, "", 15, 15, 1);
//  vector<Point> bounds = {Point(149,120), Point(439,112), Point(445,348), Point(159,361)};
  vector<Mat> samples;
  de.getImages(samples);
  
  int i = 0;
  for(auto sample: samples){
    //binary
    Mat binary;
    adaptiveThreshold(sample, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 15);
    
    //refit
    DataExtractor::refitSample(sample, binary, binary);
    //may have to process data before writing
    imwrite(savePrefix+to_string(i)+".png", binary);
    i++;
  }
}

void testMnistReader(string dataFile, string labelFile){
  vector<pair<Mat, int> > samples;
  MnistReader mr;
  mr.readData(dataFile, labelFile, samples);
}

void runMnistReader(string dataFile, string labelFile, string topFolder){
  vector<pair<Mat, int> > samples;
  MnistReader mr;
  mr.readData(dataFile, labelFile, samples);
  
  /*
  //create top folder
  string systemCall = "mkdir " + topFolder;
  if(!system(systemCall.c_str())){
    exit(-1);
  }
  
  //create sub folders
  for(int i = 0; i < 10; i++){
    systemCall = "mkdir " + topFolder + "/" + to_string(i);
    if(!system(systemCall.c_str())){
      exit(-1);
    }
  }
   */
  
  //hold counts of each label
  vector<int> counts = vector<int>(10, 0);
  
  for(auto sample: samples) {
    if(counts[sample.second] > 500) continue;
    
    string fileName = topFolder + "/" + to_string(sample.second) + "/" + to_string(counts[sample.second]) + ".png";
    //imwrite(fileName, sample.first);
    
    counts[sample.second]++;
  }
  
  cout << "complete!" << endl;
}


void testOcr(){
  Ocr ocr = Ocr("caffe-model/equation_solver.prototxt", "caffe-model/snapshot_iter_5000.caffemodel");
  Mat image = imread("/Users/Terna/Desktop/operators/multiply/4.png", IMREAD_GRAYSCALE);
  ocr.detectLetter(image, false);
}

void testSystem(){
  string protoFile = "/home/pi/EquationSolver/equation_solver/src/caffe-model/equation_solver.prototxt";
  string modelFile = "/home/pi/EquationSolver/equation_solver/src/caffe-model/snapshot_iter_5000.caffemodel";
  
  Mat image = imread("/home/pi/EquationSolver/equation_solver/src/data/test1.png", IMREAD_GRAYSCALE);
  
  CharactedDetector cd(protoFile, modelFile);
  vector<char> characters;
  cd.detectCharacters(image, characters);
  cout << characters.size() << " characters found " << endl;
  for(auto c: characters)
    cout << c << " "; 
  cout << endl;
}

int main(int argc, const char * argv[]) {
//  testDataExtractor("data/multiply.jpg");
//  runDataExtractor("data/divide.jpg","/Users/Terna/Desktop/operators/divide/");
  
//  testMnistReader("/Users/Terna/Downloads/train-images-idx3-ubyte", "/Users/Terna/Downloads/train-labels-idx1-ubyte");
  //runMnistReader("/Users/Terna/Downloads/train-images-idx3-ubyte", "/Users/Terna/Downloads/train-labels-idx1-ubyte", "/Users/Terna/Desktop/mnistdata");
  
//  testOcr();
  testSystem();
}
