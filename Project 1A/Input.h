#ifndef INPUT_H
#define INPUT_H

#include<iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


class inspect
{
private:
   string message;
   int index;

public:
	inspect() { ; };					 
	bool isOperator(char ch) const;			
	string OptimizeInput(string input);			
	void InspectOperator(string text);		 
	string Processinput(string input);		
	void Error_Report(const string& theMessage, int theIndex);		

};

#endif