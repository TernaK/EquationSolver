//
//  MathEngine.cpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/10/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "MathEngine.hpp"
#include <sstream>
using namespace std;

MathEngine::MathEngine(){
  
}

void setParsedExpression(std::string equationString);

void MathEngine::MathEngine::setMathOpList(std::string equationString){
  parsedExpression.clear();
  
  stringstream ss(equationString);
  for(int i = 0; i < equationString.length(); i++){
    string currentString = "";
    MathOp op;
    
    if(equationString[i] >= '0' && equationString[i] <='9'){
      while(equationString[i] >= '0' && equationString[i] <='9'){
        currentString += equationString[i];
        i++;
      }
      op = MathOp(OpType::OPERAND, OpPriority::NONE, currentString, atof(currentString.c_str()));
    }
    else{
      currentString += equationString[i];
      switch (equationString[i]) {
        case '*':
          op = MathOp(OpType::MULT, OpPriority::DIV_MUL, currentString);
          break;
        case '/':
          op = MathOp(OpType::DIV, OpPriority::DIV_MUL, currentString);
          break;
        case '+':
          op = MathOp(OpType::PLUS, OpPriority::PLUS_MINUS, currentString);
          break;
        case '-':
          op = MathOp(OpType::MINUS, OpPriority::PLUS_MINUS, currentString);
          break;
      }
      i++;
    }
    
    parsedExpression.push_back(op);
    i--;
  }
}

float MathEngine::solveFromOpList(){
  return 0;
}

float MathEngine::evaluate(std::string equationString){
  opList.clear();
  
  setMathOpList(equationString);
  return solveFromOpList();
}

void MathEngine::printParsedExpression(){
  cout << "expression: " ;
  for(int i = 0; i < parsedExpression.size(); i++){
    cout << parsedExpression[i].valueString << " ";
  }
  cout << endl;
}
