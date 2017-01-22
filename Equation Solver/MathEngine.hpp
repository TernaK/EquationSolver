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

// operand or operator type
enum OpType {
  OPERAND=0,PLUS=1,MINUS=2,MULT=3,DIV=4
};

//operator priority
enum OpPriority {
  NONE=-1,PLUS_MINUS=0, DIV_MUL=1
};

//operand and operator
typedef struct MathOp {
  OpType opType; //operand or operator
  OpPriority opPriority; //operator priority
  float value; //operand value
  std::string valueString; //operand value string
  
  MathOp(OpType _type, OpPriority _priority, std::string _valueString, float _value=0){
    opType = _type;
    opPriority = _priority;
    value = _value;
    valueString = _valueString;
  }
  MathOp(){};
} MathOp;

/* This class is used to evaluate math expressions from strings like "5 * 3 / 1 + 7".
 * No syntax checking is performed.
 */
class MathEngine {
private:
  std::stack<MathOp> operatorStack; //operator stack
  std::list<MathOp> infixOpList; //linked list of the expression in infix form
  std::vector<MathOp> parsedExpression; //array of operand and operators in expression
  
  std::string infixExpressionString;
  std::string parsedExpressionString;
  
  //set the infix expression list, call after setting the parsed expression
  void setInfixOpList();
  
  //obatain and set the parsed expression array given an expression string
  void setParsedExpression(std::string equationString);
  
  //solve the equation and return the solution
  float solveFromInfixOpList();
  
  void setParsedExpressionString();
  
  void setInfixExpressionString();
  
public:
  MathEngine();
  
  //evaluate a math expression from a string
  float evaluate(std::string equationString);

  std::string getInfixString(){ return infixExpressionString; };
  
  std::string getParsedString(){ return  parsedExpressionString; };
};

#endif /* MathEngine_hpp */
