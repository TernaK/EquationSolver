//
//  CharacterDetector.hpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef CharacterDetector_hpp
#define CharacterDetector_hpp

#include "EquationSolver.hpp"
#include "Ocr.hpp"

class CharactedDetector {
private:
  Ocr ocr;
public:
  CharactedDetector(std::string modelFile);
  void detectCharacters(const cv::Mat& image, std::vector<char> characters);
  void getCharRois(const cv::Mat& image, std::vector<cv::Rect>& rois);
};

#endif /* CharacterDetector_hpp */
