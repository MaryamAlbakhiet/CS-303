#include "Infix.h"

Infix::Infix()
{
}

int Infix::evaluate(string s, ofstream& log_file)
{

	s = convert(s);                     // Convert Operation to Single Chars 
	log_file << "Converted string:\t" << s << endl;
	unsigned int token = 0;
	while (token < s.length())
	{
		
		int prev = token;
		char c = s[token];
		if (isdigit(c))
		{
			
			int number = 0;
			while (token < s.length() && isdigit(c))
			{

				number *= 10;
				number += c - 48;
				++token;
				c = s[token];
			}
			--token;
			c = s[token];

			
			operands.push(number);
		}
		
		else if (c == '-' && (token == 0 || !isdigit(s[--prev])))
		{
			++token;
			c = s[token];

			if (isdigit(c))
			{
				int number = 0;
				while (token < s.length() && isdigit(c))
				{
					number *= 10;
					number += c - 48;
					++token;
					c = s[token];
				}
				number *= -1;
				operands.push(number);
				--token;
			}
			else
			{
				while (!operators.empty() && !operands.empty() && getPrecedence(c) < getPrecedence(operators.top()) && operators.top() != '(')
				{
					solveTop(operators, operands);
				}
				operators.push(c);
			}

		}
		else if (c == '(')
		{
			operators.push(c);
		}

		else if (c == ')')
		{
			while (!operands.empty() && !operators.empty() && operators.top() != '(')
				solveTop(operators, operands);

		}
		
		else if (isOperator(c))
		{
			while (!operators.empty() && !operands.empty() && getPrecedence(c) < getPrecedence(operators.top()) && operators.top() != '(')
			{
				solveTop(operators, operands);

			}
			if (c != '\0')
				operators.push(c);
		}
		else
		{
			std::string msg = " Invalid symbol, ";
			msg += c;
			throw std::exception(msg.c_str());
		}
		++token;
	}



	while (!operators.empty() && !operands.empty())
	{
		solveTop(operators, operands);

	}
	if (!operands.empty())
		return int(operands.top());
	else
	{
		std::string msg = "Calc error: no result left on stack";
		throw std::exception(msg.c_str());
	}


	return int(operands.top());
}



void Infix::solveTop(stack<char>& operators, stack<double>& operands)
{
	int result = 0;

	
	char c;
	if (!operators.empty())
	{
		c = operators.top();
	}
	else
	{
		std::string msg = "no operator left to calculate ";
		throw std::exception(msg.c_str());
	}
	
	if (isBinaryOp(c))
	{
		double i1;
		if (!operands.empty())
			i1 = operands.top();
		else
		{
			std::string msg = "no operand left to calculate ";
			msg += c;
			throw std::exception(msg.c_str());
		}

		operands.pop();
		double i2;
		if (!operands.empty())
			i2 = operands.top();
		else
		{
			std::string msg = " no operand left to calculate ";
			msg += c;
			throw std::exception(msg.c_str());
		}
		operands.pop();
		result = int(binarySolve(i1, i2, c));
		log_file << "Pushing " << result << " to operands\n";
		operands.push(result);
		operators.pop();
	}

	

	else
	{
		double i1 = 0;
		if (!operands.empty())
			i1 = operands.top();

		
		else
		{
			std::string msg = "no operand left to calculate " + to_string(c);
			throw std::exception(msg.c_str());
		}
		operands.pop();
		result = int(unarySolve(i1, c));
		log_file << "Pushing " << result << " to operands\n";
		operands.push(result);
		operators.pop();


	}

}




bool Infix::isBinaryOp(char c)
{
	return BINARY_OPERATORS.find(c) != string::npos;
}

bool Infix::isOperator(char c)
{
	return OPERATORS.find(c) != string::npos;
}

int Infix::getPrecedence(char c)
{
	switch (c)
	{
	case '(':
	case ')':
		return 9;
	case '!':
	case '@':
	case '#':
		return 8;
	case '^':
		return 7;
	case '*':
	case '/':
	case '%':
		return 6;
	case '+':
	case '-':
		return 5;
	case '>':
	case '$':
	case '<':
	case '~':
		return 4;
	case '`':
	case ':':
		return 3;
	case '&':
		return 2;
	case '|':
		return 1;

		// Error, invalid operator
	default:
		string msg = "Calc error:  Symbol '";
		msg += c;
		msg += "' not valid for calculation, exiting calculation";
		throw std::exception(msg.c_str());
	}
}

string Infix::convert(string s)
{
	string result = s;
	
	replaceAll(result, "++", "@");
	replaceAll(result, "--", "#");
	replaceAll(result, ">=", "$");
	replaceAll(result, "<=", "~");
	replaceAll(result, "==", "`");
	replaceAll(result, "!=", ":");
	replaceAll(result, "&&", "&");
	replaceAll(result, "||", "|");
	return result;
}


void Infix::replaceAll(string& str, const string& from, const string& to)
{
	if (from.empty())
		return;
	size_t start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}


int Infix::unarySolve(int i, char c)
{
	switch (c)
	{
	case '!':
		return !i;
	case '@':
		return ++i;
	case '#':
		return --i;
	case '-':
		return -i;
	case '(':
		return i;

		
	default:
		string msg = "Calc Error:  Symbol '";
		msg += c;
		msg += "' not valid for calculation, exiting calculation";
		throw std::exception(msg.c_str());
	}
}



double Infix::binarySolve(double i1, double i2, char c)
{
	switch (c)
	{
	case '-':
		return(i2 - i1);
	case '^':
		return pow(i2, i1);
	case '*':
		return i2 * i1;
	case '/':
		
		if (i1 == 0)
		{
			string msg = "Calc Error:  Divide by Zero, ";
			msg += to_string(int(i2));
			msg += "/";
			msg += to_string(int(i1));
			throw std::exception(msg.c_str());
		}
		else
			return i2 / i1;
	case '%':
		return (int)i2 % (int)i1;
	case '+':
		return i2 + i1;
	case '>':
		return i2 > i1;
	case '$':
		return i2 >= i1;
	case '<':
		return i2 < i1;
	case '~':
		return i2 <= i1;
	case '`':
		return i2 == i1;
	case ':':
		return i2 != i1;
	case '&':
		return i2 && i1;
	case '|':
		return i2 || i1;


	default:
		string msg = "Calc Error:  Symbol '";
		msg += c;
		msg += "' not valid for calculation, exiting calculation";
		throw std::exception(msg.c_str());
	}
}

void Infix::clearData()
{

	while (!operands.empty())
		operands.pop();

	while (!operators.empty())
		operators.pop();

	return;

}