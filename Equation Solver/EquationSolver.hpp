//
//  EquationSolver.hpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef EquationSolver_h
#define EquationSolver_h

#include <stdio.h>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <vector>


enum OperandType {
  LIERTAL=0, OPERATION=1
};

typedef struct Operand {
  OperandType opType;
  char value;
} Operand;

#endif /* EquationSolver_h */
