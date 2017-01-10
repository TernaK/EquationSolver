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
  
  setParsedExpressionString();
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
        while((top.opPriority >= op.opPriority) && !operatorStack.empty()){
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
  
  setInfixExpressionString();
}

float MathEngine::solveFromInfixOpList(){
  list<MathOp>::iterator it1 = infixOpList.begin();
  list<MathOp>::iterator it2 = infixOpList.begin();
  list<MathOp>::iterator it3 = infixOpList.begin();
  it1++; it1++;
  it2++;
  
  float subResult = 0;
  while (true) {
    
    if(it1->opType != OPERAND){
      
      switch (it1->opType) {
        case MULT:
          subResult = it3->value*it2->value;
          break;
        case DIV:
          subResult = it3->value/it2->value;
          break;
        case PLUS:
          subResult = it3->value+it2->value;
          break;
        case MINUS:
          subResult = it3->value-it2->value;
          break;
        case OPERAND: break;//never gets here
      }
      
      //cout << it3->value << it1->valueString << it2->value << " = " << subResult << endl;
      if(infixOpList.size() == 3){
        break;
      }
      
      it3->value = subResult;
      infixOpList.erase(it1);
      infixOpList.erase(it2);
    }
    else{
      it1++; it2++; it3++;
      continue;
    }
    
    it1 = infixOpList.begin();
    it2 = infixOpList.begin();
    it3 = infixOpList.begin();
    it1++; it1++;
    it2++;
  }
  
  return subResult;//dummy
}

float MathEngine::evaluate(std::string equationString){
  
  setParsedExpression(equationString);
  setInfixOpList();
  return solveFromInfixOpList();
}

void MathEngine::setParsedExpressionString(){
  parsedExpressionString = "";
  for(int i = 0; i < parsedExpression.size(); i++){
    parsedExpressionString += parsedExpression[i].valueString + " ";
  }
}


void MathEngine::setInfixExpressionString(){
  infixExpressionString = "";
  list<MathOp>::iterator iter;
  for(iter = infixOpList.begin(); iter != infixOpList.end(); iter++){
    infixExpressionString += iter->valueString + " ";
  }
}
