//
//  main.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/7/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "CharacterDetector.hpp"
#include "MathEngine.hpp"
using namespace std;
using namespace cv;

void testMathEnginEvaluate(){

  MathEngine engine;
  engine.evaluate("5*7");
  engine.printParsedExpression();
  engine.evaluate("34*7/20");
  engine.printParsedExpression();
}

void testCharactedDetector(){
  Mat image = imread("/Users/Terna/Desktop/test1.png", IMREAD_GRAYSCALE);
  
  CharactedDetector detector("dummy");
  vector<char> chars;
  detector.detectCharacters(image, chars);
}

int main(int argc, const char * argv[]) {

  testMathEnginEvaluate();
}
