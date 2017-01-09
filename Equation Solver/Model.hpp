//
//  Model.hpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef Model_hpp
#define Model_hpp

#include "EquationSolver.hpp"

class Model{
private:
  cv::Mat w1, b1, c2, b2;
public:
  Model();
  Model(float *_w1, float *_b1, float *_w2, float *_b2, int nx, int nh, int ny);
  int feedForward(float *x, float *y);
};

#endif /* Model_hpp */
