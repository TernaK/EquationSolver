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
#include <stack>
#include <list>

enum OpType {
  OPERAND=0,PLUS=1,MINUS=2,MULT=3,DIV=4
};
enum OpPriority {
  NONE=-1,PLUS_MINUS=0, DIV_MUL=1
};

typedef struct MathOp {
  OpType opType;
  OpPriority opPriority;
  float value;
  std::string valueString;
  MathOp(OpType _type, OpPriority _priority, std::string _valueString, float _value=0){
    opType = _type;
    opPriority = _priority;
    value = _value;
    valueString = _valueString;
  }
  MathOp(){};
} MathOp;


class MathEngine {
private:
  std::stack<MathOp> operatorStack;
  std::list<MathOp> opList;
  std::vector<MathOp> parsedExpression;
  
  void setMathOpList(std::string equationString);
  float solveFromOpList();
  void setParsedExpression(std::string equationString);
public:
  MathEngine();
  float evaluate(std::string equationString);
  void printParsedExpression();
};

#endif /* MathEngine_hpp */
