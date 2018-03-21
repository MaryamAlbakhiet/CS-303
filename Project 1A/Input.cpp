#include "Input.h"
#include <cctype>



bool inspect::isOperator(char ch) const
{
	return (ch == '!') || (ch == '+') || (ch == '-') || (ch == '^') || (ch == '*') || (ch == '/') || (ch == '%') || (ch == '>') || (ch == '<') || (ch == '=') || (ch == '&') || (ch == '|') || (ch == '(') || (ch == ')');
}

string inspect::OptimizeInput(string input)
{
	int balofBracket = 0;
	if (input.empty())
		Error_Report("Error Message : the input is empty!", -1);
	string output;
	int k = 0;

	
	while (input[k] == ' ' || input[k] == '\t' || input[k] == '\n')
	{
		k++;
	}
	if (k == input.size())
		Error_Report("Error Message : the input is empty!", -1);

	for (unsigned int i = k; i < input.size(); i++)
	{
		if (isdigit(input[i]))
		{
			output.push_back(input[i]);
			continue;
		}
		if (isOperator(input[i]))
		{
			if (input[i] == '(')
				balofBracket++;
			if (input[i] == ')')
				balofBracket--;
			if (balofBracket < 0)
				Error_Report("Error Message: Expression cannot start with a closing parenthesis ", i);
			output.push_back(input[i]);
			continue;
		}
		if (input[i] == ' ' || input[k] == '\t')
		{
			if (isdigit(input[i - 1]))
			{
				while (i < input.size())
				{
					if (input[i + 1] == ' ')
					{
						i++;
					}
					else
						break;
				}
				if (i != (input.size() - 1))
				{
					if (isdigit(input[i + 1]))
						Error_Report("Error Message: Two Operands in a row", i + 1);
				}
			}
		}
		if (!isdigit(input[i]) && !isOperator(input[i]) && (input[i] != ' ') && (input[i] != '\t'))
			Error_Report("Error Message: Invail symbol", i);
	}
	if (balofBracket > 0)
		Error_Report("Error Message: there are more opening parenthesis than closing parenthesis", -1);
	return output;
}

void inspect::InspectOperator(string text)
{
	if (text.empty())
		return;

	int Indicator = 2;
	if (isdigit(text[0]))
		Indicator = 1;
	
	for (unsigned int a = 0; a < text.size(); a++)
	{

		if (isdigit(text[a]))
		{
			if ((a + 1) < text.size())
			{
				if ((text[a + 1] == '+') || (text[a + 1] == '-') || (text[a + 1] == '*') || (text[a + 1] == '/') || (text[a + 1] == '%') || (text[a + 1] == '<') || (text[a + 1] == '>') || (text[a + 1] == '=') || (text[a + 1] == '|') || (text[a + 1] == '&') || (text[a + 1] == '^'))
				{
					Indicator = 1;
				}
				else if ((a + 2) < text.size() && (text[a + 1] == '!') && (text[a + 2] == '='))
					Indicator = 1;
				else
					Indicator = 0;
			}
			continue;
		}

		if (text[a] == ')')
		{

			if (Indicator == 0)
			{
				if ((a + 1) < text.size())
				{
					if (isOperator(text[a + 1]))
						continue;
					else
						Error_Report("Error Message : the digit can't follow with the closing parethesis", a + 1);
				}
			}
			else
				Error_Report("Error Message: there shouldn't be closing parenthesis in such position", a);
		}

		if (Indicator == 0) Indicator = 1;

		if ((text[a] == '^') || (text[a] == '*') || (text[a] == '/') || (text[a] == '%'))
		{
			if (Indicator == 1)
			{
				Indicator = 2;
				continue;
			}
			else
				Error_Report("Error Message: binary operator cannot appear in such position", a);
		}

		if ((text[a] == '=') || (text[a] == '&') || (text[a] == '|'))
		{
			if ((Indicator == 1) && (a < text.size() - 1))
			{
				if (text[a + 1] == text[a])
				{
					a++;
					Indicator = 2;
					continue;
				}
				else
					Error_Report("Error Message: Incomplete Operator", a);
			}
			else
				Error_Report("Error Message : Two binary operators in a row  ", a);
		}

		if (text[a] == '!')
		{
			if (Indicator == 2)
			{
				continue;
			}
			else if ((Indicator == 1) && (a < text.size() - 1))
			{
				if (text[a + 1] == '=')
				{
					a++;
					Indicator = 2;
					continue;
				}
				else
					Error_Report("Error Message: incomplete operator", a);
			}
			else
				Error_Report("Error Message: operator cannot appear in such position", a);
		}

		if ((text[a] == '+') || (text[a] == '-'))
		{
			if (Indicator == 1 && (a < text.size() - 1))
			{
				if ((text[a + 1] == '(') || (text[a + 1] == '!') || isdigit(text[a + 1]))
				{
					Indicator = 2;
					continue;
				}
				else
					Error_Report("Error Message: A unary operand can’t be followed by a binary operator ", a + 1);
			}
			else if (Indicator == 2)
			{
				if ((a < text.size() - 1) && (text[a + 1] == text[a]))
				{
					a++;
					continue;
				}
				else if (a == 0 && isdigit(text[a + 1]))
				{
					Indicator = 1;
					continue;
				}
				else
				{
					if ((a < text.size() - 1) && (text[a + 1] == '('))
					{
						a++;
						continue;
					}
					else if ((a < text.size() - 1) && isdigit(text[a + 1]) && text[a - 1] == '(')
					{
						Indicator = 1;
						continue;
					}
					else
						Error_Report("Error Message: two binary operators in a row", a);
				}
			}
			else
				Error_Report("Error Message:` operator cannot appear in such position", a);
		}

		if ((text[a] == '>') || (text[a] == '<'))
		{
			if (Indicator == 1)
			{
				if ((a < text.size() - 1) && (text[a + 1] == '='))
				{
					a++;
					Indicator = 2;
					continue;
				}
				else
				{
					Indicator = 2;
					continue;
				}
			}
			else
				Error_Report("Error Message : Expression cannot start with a binary operator", a);
		}

		if (text[a] == '(')
		{
			if (Indicator == 2)
			{
				if ((a < text.size() - 1) && (isdigit(text[a + 1])))
				{
					Indicator = 1;
					continue;
				}
				else
				{
					continue;
				}
			}
			else
				Error_Report("Error Message: opening parenthesis cannot appear in such position", a);
		}
	}
	if (text[text.size() - 1] != ')' && !isdigit(text[text.size() - 1]))
		Error_Report("Error Message: lose the last operand at the end.", -1);
}



string inspect::Processinput(string input)
{
	string output = OptimizeInput(input);
	InspectOperator(output);
	return output;
}

void inspect::Error_Report(const string& theMessage, int theIndex)
{
	std::string msg;

	if (theIndex != -1)
		msg = theMessage + " @ char " + to_string(theIndex);
	else
		msg = theMessage;

	throw std::exception(msg.c_str());

}