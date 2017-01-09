//
//  ModelLoader.hpp
//  Equation Solver
//
//  Created by Terna Kpamber on 1/8/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef ModelLoader_hpp
#define ModelLoader_hpp

#include "EquationSolver.hpp"
#include "Model.hpp"

class ModelLoader{
private:
public:
  static void loadModel(std::string fileName, Model& model);
};

#endif /* ModelLoader_hpp */
