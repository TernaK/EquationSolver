//
//  MathEngine.cpp
//  EquationSolver
//
//  Created by Terna Kpamber on 1/10/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "MathEngine.hpp"
using namespace std;

MathEngine::MathEngine(){
}

void MathEngine::MathEngine::setParsedExpression(std::string equationString){
  parsedExpression.clear();
  
  for(int i = 0; i < equationString.length(); i++){
    string currentString = "";
    MathOp op;
    
    //keep reading integers and concatenate them until
    //we hit a non-integer
    if(equationString[i] >= '0' && equationString[i] <='9'){
      while(equationString[i] >= '0' && equationString[i] <='9'){
        currentString += equationString[i];
        i++;
      }
      op = MathOp(OpType::OPERAND, OpPriority::NONE, currentString, atof(currentString.c_str()));
    }
    
    //take single operator characters
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
    i--;//remove one to keep the for loop in sequence
  }
}

void MathEngine::setInfixOpList(){
  infixOpList.clear();
  
  for(auto op: parsedExpression){
    //if operand, push to infix
    if(op.opType == OpType::OPERAND){
      infixOpList.push_back(op);
    }
    //if operator
    else {
      //if stack is empty, push operator
      if(operatorStack.empty()){
        operatorStack.push(op);
      }
      else{
        MathOp top = operatorStack.top();
        //while top is higher priority, push to infix, pop from stack
        while((top.opPriority > op.opPriority) && !operatorStack.empty()){
          infixOpList.push_back(top);
          operatorStack.pop();
          //reset the top
          if(!operatorStack.empty())
            top = operatorStack.top();
        }
        operatorStack.push(op);
      }
    }
  }
  while(!operatorStack.empty()){
    infixOpList.push_back(operatorStack.top());
    operatorStack.pop();
  }
  
}

float MathEngine::solveFromInfixOpList(){
  list<MathOp>::iterator it1;
  list<MathOp>::iterator it2;
  list<MathOp>::iterator it3;
  
  return 0;
}

float MathEngine::evaluate(std::string equationString){
  
  setParsedExpression(equationString);
  setInfixOpList();
  return solveFromInfixOpList();
}

void MathEngine::printParsedExpression(){
  cout << "parsed expression: " ;
  for(int i = 0; i < parsedExpression.size(); i++){
    cout << parsedExpression[i].valueString << " ";
  }
  cout << endl;
}


void MathEngine::printInfixOpList(){
  cout << "parsed infix expression: " ;
  list<MathOp>::iterator iter;
  for(iter = infixOpList.begin(); iter != infixOpList.end(); iter++){
    cout << iter->valueString << " ";
  }
  cout << endl;
}
