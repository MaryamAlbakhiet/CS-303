#ifndef INFIX_H
#define INFIX_H

#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <sstream>
#include <cctype>
#include <iostream>
#include <fstream>
#include <stdexcept>
using namespace std;

class Infix {

public:
   Infix();
   int evaluate(string s, ofstream& log_file);
   void clearData();

private:
	void solveTop(stack<char>& operators, stack<double>& operands);
	bool isBinaryOp(char c);
	bool isOperator(char c);
	int unarySolve(int i, char c);
	double binarySolve(double i1, double i2, char c);
	int getPrecedence(char c);
	ofstream log_file;
    void replaceAll(string& str, const string& from, const string& to);
	string convert(string s);
	const string OPERATORS = "!@#-^*/%+>$<~`:&|";
	const string BINARY_OPERATORS = "^* / %+->$<~`:& | ";
	stack<char> operators;
	stack<double> operands;
};

#endif