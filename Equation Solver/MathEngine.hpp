//
//  MathEngine.hpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/10/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef MathEngine_hpp
#define MathEngine_hpp

#include "EquationSolver.hpp"

typedef struct adt{
  
} adt;

class MathEngine {
private:
  adt adt;
  void stringToAdt(std::string equationString);
  float solveFromAdt();
public:
  MathEngine();
  float evaluate(std::string equationString);
};

#endif /* MathEngine_hpp */
