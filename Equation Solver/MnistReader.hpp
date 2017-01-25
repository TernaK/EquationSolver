//
//  MnistReader.hpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/24/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef MnistReader_hpp
#define MnistReader_hpp

#include "EquationSolver.hpp"

class MnistReader {
public:
  MnistReader(){};
  bool readData(std::string dataFile, std::string labelFile, std::vector<std::pair<cv::Mat,int>>& samples);
};

#endif /* MnistReader_hpp */
