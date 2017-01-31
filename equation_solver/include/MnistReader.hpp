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

  /* take a data file, a label file, then read all images
   * dataFile: the path to the data file
   * labelFile: the path to the label file
   * samples:output vector of label,image Mat pairs read from the provided files
   */
  bool readData(std::string dataFile, std::string labelFile, std::vector<std::pair<cv::Mat,int>>& samples);
};

#endif /* MnistReader_hpp */
