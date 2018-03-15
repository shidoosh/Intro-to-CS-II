//
//  eval.cpp
//  CS32 - HOMEWORK 2
//
//  Created by Stefanie Shidoosh on 5/1/17.
//  Copyright © 2017 Stefanie Shidoosh. All rights reserved.
//

#include <stack>
#include <string>
#include <cassert>
#include <iostream>

bool isValidInfix(std::string infix);
std::string infixToPostfix(std::string infix);
std::string eraseSpaces(std::string infix);
bool solve(std::string postfix);

int evaluate(std::string infix, std::string& postfix, bool& result)
{
    // Evaluates a boolean expression
    //   If infix is a syntactically valid infix boolean expression,
    //   then set postfix to the postfix form of that expression, set
    //   result to the value of the expression, and return zero.  If
    //   infix is not a syntactically valid expression, return 1; in
    //   that case, postfix may or may not be changed, but result must
    //   be unchanged.)
    int returning = 0;
    
    if(!isValidInfix(infix))
        return 1;
    
    if(isValidInfix(infix))
    {
        postfix = infixToPostfix(infix);
        result = solve(postfix);
        return 0;
    }

    return returning;
}


//helper functions! :)
bool isValidInfix(std::string infix)
{
    bool result = true;

    infix = eraseSpaces(infix);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    int openParenCount = 0;
    int closedParenCount = 0;
    for(int j = 0; j < infix.size(); j++)  //Check that each open parentheses is approprately closed.
    {
        if(infix[j]=='(')
        {
            openParenCount++;
        }
        if(infix[j]==')')
        {
            closedParenCount++;
        }
    }
    if(openParenCount!=closedParenCount)
        return false;
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if(infix[0] != 'T' && infix[0] != 'F' && infix[0] != '(' && infix[0] != '!') //If string does not begin with valid char, return false.
        return false;
    
    for(int i = 0; i < infix.size(); i++)   //Search the infix string.
    {
        if(infix[i]=='!')    //!!!!!!!!!!!!!!!!!!!!
        {
            if(infix.size()==1 && i==0)
                return false;
            
            if(i!=0)
            {
                if(infix[i-1] !='&' && infix[i-1] !='|' && infix[i-1] !='!' && infix[i-1] != '(')
                    return false;
            }
            if(infix[i+1] != 'T' && infix[i+1]!= 'F' && infix[i+1] != '(' && infix[i+1] != '!')
                return false;
        }
        
        if(infix[i]=='T')
        {
            if(infix.size()==1 && i==0)
                return true;
            
            if(infix.size()>1)
            {
                if(i+1!=infix.size())
                {
                    if(infix[i+1] != '&' && infix[i+1] != '|' && infix[i+1] != ')')
                    {
                        return false;
                    }
                }
                if(infix[i-1]==')')
                    return false;
            }
        }
        
        if(infix[i]=='F')
        {
            if(infix.size()==1 && i==0)
                return true;

            if(infix.size()>1)
            {
                if(i+1!=infix.size())
                {
                    if(infix[i+1] != '&' && infix[i+1] != '|' && infix[i+1] != ')')
                    {
                        return false;
                    }
                }
                if(infix[i-1]==')')
                    return false;
            }
        }
        
        if(infix[i]=='(')
        {
            if(infix.size()==1 && i==0)
                return false;

            if(infix[i+1] != 'T' && infix[i+1]!= 'F' && infix[i+1]!= '!' && infix[i+1] != '(')
                return false;
            
            if(i!=0)
            {
                if(infix[i-1] !='!' && infix[i-1]!='&' && infix[i-1]!='|' && infix[i-1] != '(')
                    return false;
            }

        }
        
        if(infix[i]==')')
        {
            if(infix.size()==1 && i==0)
                return false;
            if(i+1!=infix.size())
            {
                if(infix[i+1]!= '&' && infix[i+1]!= '|' && infix[i+1] != ')')
                {
                    return false;
                }
            }
            if(infix[i-1]!='T' && infix[i-1]!='F' && infix[i-1] != ')')
                return false;
        }
        
        if(infix[i]=='&')
        {
            if(infix.size()==1 && i==0)
                return false;
            if(infix[i+1]!='T' && infix[i+1]!='F' && infix[i+1]!='!' && infix[i+1]!='(')
                return false;
            if(infix[i-1]!='T' && infix[i-1]!='F' && infix[i-1]!=')'  )
                return false;
        }
        
        if(infix[i]=='|')
        {
            if(infix.size()==1 && i==0)
                return false;
            
            if(infix[i+1] != 'T' && infix[i+1] != 'F' && infix[i+1] != '!')
                return false;
            if(infix[i-1] != 'T'&& infix[i-1] != 'F'&& infix[i-1] != ')')
                return false;
        }
        
    }
    return result;
}

std::string infixToPostfix(std::string infix)
{
     std::stack<char> operatorStack;
     std::string postfix = ""; // Initialize postfix as empty string.
    
     for(int i = 0;i< infix.size();i++)
     {
         char ch = infix[i];
         switch (ch) {
             case 'T':
             case 'F':
                 postfix+=ch;
                 break;
             case '(':
                 operatorStack.push(ch);
                 break;
             case ')':
                 while(operatorStack.top()!='(')
                 {
                     postfix+=operatorStack.top();
                     operatorStack.pop();
                 }
                 operatorStack.pop();
                 break;
             case '!':
                 operatorStack.push(ch);
                 break;
             case '&':
             case '|':
                 while(!operatorStack.empty() && operatorStack.top()!='(' && ch >= operatorStack.top())
                 {
                     postfix+=operatorStack.top();
                     operatorStack.pop();
                 }
                 operatorStack.push(ch);
                 break;
             default:
                 break;
         }
     }
    while (!operatorStack.empty())
    {
        postfix+=operatorStack.top();
        operatorStack.pop();
    }
    return postfix;
}

bool solve(std::string postfix)
{
    bool result;
    std::stack<bool> operandStack;
    
    for(int i = 0; i < postfix.size(); i++)
    {
        if(postfix[i] == 'T' || postfix[i] == 'F')
        {
            if(postfix[i]=='T')
                operandStack.push(1);
            if(postfix[i]=='F')
                operandStack.push(0);
        }
        else
        {
            if(operandStack.empty()==false)
            {
                bool operand2 = operandStack.top();
                operandStack.pop();
                
                if(postfix[i] == '!')
                    operandStack.push(!operand2);
                else
                {
                    if(operandStack.empty()==true)
                        return false;
                    bool operand1 = operandStack.top();
                    operandStack.pop();
                    
                    if(postfix[i]=='&')
                    {
                        operandStack.push(operand1 && operand2);
                    }
                    else if(postfix[i]=='|')
                    {
                        operandStack.push(operand1 || operand2);
                    }
                }
            }
        }
    }
    if(operandStack.size()==1)
    {
        result = operandStack.top();
    }
    
    return result;
}

std::string eraseSpaces(std::string infix)
{
    std::string noSpaces = "";
    for(int i = 0; i < infix.size(); i++)
    {
        if(infix[i] != ' ')
        {
            noSpaces+=infix[i];
        }
    }
    infix = noSpaces;
    return infix;
}

