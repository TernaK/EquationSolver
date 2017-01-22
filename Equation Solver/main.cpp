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
  Mat image = imread("/Users/Terna/Desktop/test1.png", IMREAD_GRAYSCALE);
  
  CharactedDetector detector("dummy");
  vector<char> chars;
  detector.detectCharacters(image, chars);
}

void testDataExtractor(){
  Mat image = imread("/Users/Terna/Desktop/test2.jpg", IMREAD_GRAYSCALE);
  DataExtractor de = DataExtractor(image, "", 5, 6, 1);
  vector<Point> bounds = {Point(149,120), Point(439,112), Point(445,348), Point(159,361)};
  vector<Mat> samples;
  de.getImages(samples, bounds);
  
  for(auto sample: samples){
    namedWindow("extracted");
    imshow("extracted", sample) ;
    waitKey();
  }
}

int main(int argc, const char * argv[]) {
  testDataExtractor();
}
