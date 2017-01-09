//
//  main.cpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/7/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "CharacterDetector.hpp"
using namespace std;
using namespace cv;

int main(int argc, const char * argv[]) {
  
  Mat image = imread("/Users/Terna/Desktop/test1.jpg");

  CharactedDetector detector("dummy");
  vector<char> chars;
  detector.detectCharacters(image, chars);
  
}
