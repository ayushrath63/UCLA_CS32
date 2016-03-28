#include <string>
#include <stack>
#include <cctype>
#include "Map.h"

using namespace std;

//returns true if precedence of one (operator) is <= precedence of two (operator)
bool comparePrecendence(char one, char two)
{
	if (one == '*' || one == '/')
	{
		if (two == '+' || two == '-')
			return false;
	}
	return true;
}

//get result of operation ore return false if division by 0
bool getResult(int op1, int op2, char operation, int& result)
{
	if (op2 == 0 && operation == '/')
		return false;
	else
	{
		switch (operation)
		{
		case '+':
			result = op1 + op2;
			break;
		case '-':
			result = op1 - op2;
			break;
		case '*':
			result = op1 * op2;
			break;
		case '/':
			result = op1 / op2;
			break;
		}
	}
	return true;
}

//generates RPN expression from infix expression
void inToPost(string infix, string& postfix)
{
	postfix = "";
	stack<char> opers;

	for (int i = 0; i < infix.size(); i++)
	{
		switch (infix[i])
		{
		default:
			if(isalpha(infix[i]))
				postfix += infix[i];
			break;
		case '(':
			opers.push(infix[i]);
			break;
		case ')':
			//pop stack until matching '('
			while (opers.top() != '(')
			{
				postfix += opers.top();
				opers.pop();
			}
			opers.pop(); //remove ')'
			break;
		case '*':
		case '-':
		case '+':
		case '/':
			while ( (opers.size() != 0) && (opers.top() != '(') && comparePrecendence(infix[i], opers.top()) )
			{
				postfix += opers.top();
				opers.pop();
			}
			opers.push(infix[i]);
			break;
		}
	}

	//while stack not empty
	while (opers.size())
	{
		postfix += opers.top();
		opers.pop();
	}
}

//checks if number of operators and operands in postfix expression are valid
bool validatePfix(string postfix)
{	
	int count = 0;
	for (int i = 0; i < (int)postfix.size(); i++)
	{
		if (isalpha(postfix[i]))
		{
			if (!islower(postfix[i]))
				return false;
			count++;
		}
		else if (postfix[i] == '+' || postfix[i] == '-' || postfix[i] == '/' || postfix[i] == '*')
			count--;
	}
	if (count == 1)
		return true;
	else return false;
}

int evaluate(string infix, const Map& values, string& postfix, int& result)
{
	string tmp;
	inToPost(infix, tmp);
	stack<int> operands;

	//set postfix to tmp if expression is valid
	if (!validatePfix(tmp))
		return 1;
	postfix = tmp;

	//return 2 if postfix contains operands not in map
	for (int i = 0; i < postfix.size(); i++)
	{
		if (isalpha(postfix[i]) && !values.contains(postfix[i]))
			return 2;
	}

	for (int i = 0; i < postfix.size(); i++)
	{
		switch (postfix[i])
		{
		case '+':
		case '-':
		case '*':
		case '/':
			//evaluate part of expression if operator reached
			if (operands.size() >= 2)
			{
				int op1, op2, tmpResult;
				op1 = operands.top();
				operands.pop();
				op2 = operands.top();
				operands.pop();
				//find result of operation, make sure not dividing by 0
				if (getResult(op2, op1, postfix[i], tmpResult))
					operands.push(tmpResult);
				//return 3 if division by sero occurs
				else return 3;

			}
			break;
		default:
			//push operands onto stack
			int op;
			values.get(postfix[i], op);
			operands.push(op);
			break;
		}
	}

	//if only one item left on stack, return 0, set result
	if (operands.size() == 1)
	{
		result = operands.top();
		operands.pop();
		return 0;
	}
	return 1;
}

///TESTS BELOW

#include <iostream>
#include <cassert>

int main()
{
	char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
	int  vals[] = { 3,  -9,   6,   2,   4,   1 };
	Map m;
	for (int k = 0; vars[k] != '#'; k++)
		m.insert(vars[k], vals[k]);
	string pf;
	int answer;
	inToPost("y +o *(   a-u) ", pf);
	int thing = evaluate("y +o *(   a-u)  ", m, pf, answer);
	assert(evaluate("a+ e", m, pf, answer) == 0 &&
		pf == "ae+"  &&  answer == -6);
	answer = 999;
	assert(evaluate("", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a i", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("ai", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("()", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("y(o+u)", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a+E", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("(a+(i-o)", m, pf, answer) == 1 && answer == 999);
	// unary operators not allowed:
	assert(evaluate("-a", m, pf, answer) == 1 && answer == 999);
	assert(evaluate("a*b", m, pf, answer) == 2 &&
		pf == "ab*"  &&  answer == 999);
	assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0 &&
		pf == "yoau-*+" &&  answer == -1);
	answer = 999;
	assert(evaluate("o/(y-y)", m, pf, answer) == 3 &&
		pf == "oyy-/"  &&  answer == 999);
	assert(evaluate(" a  ", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	assert(evaluate("((a))", m, pf, answer) == 0 &&
		pf == "a"  &&  answer == 3);
	cout << "Passed all tests" << endl;
}