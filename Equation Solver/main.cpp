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
  Mat image = imread("data/test1.png", IMREAD_GRAYSCALE);
  
  CharactedDetector detector("dummy");
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
    Mat binary;
    adaptiveThreshold(sample, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 15);
    namedWindow("extracted");
    imshow("extracted", binary) ;
    waitKey();
  }
}

void runDataExtractor(string fileName, string savePrefix){
  
  Mat image = imread(fileName, IMREAD_GRAYSCALE);
  resize(image, image, image.size()/6);
  DataExtractor de = DataExtractor(image, "", 15, 15, 1);
  vector<Point> bounds = {Point(149,120), Point(439,112), Point(445,348), Point(159,361)};
  vector<Mat> samples;
  de.getImages(samples);
  
  int i = 0;
  for(auto sample: samples){
    Mat binary;
    adaptiveThreshold(sample, binary, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 25, 15);
    //may have to process data before writing
    imwrite(savePrefix+to_string(i)+".jpg", binary);
    i++;
  }
}

int main(int argc, const char * argv[]) {
//  testDataExtractor("data/multiply.jpg");
  runDataExtractor("data/multiply.jpg","/Users/Terna/Desktop/operators/multiply/");
}
