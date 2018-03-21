#include <iostream>
#include <string> 
#include <stack>
#include "Infix.h"
#include "Input.h"

using namespace std;


void display(Infix& ie, string& user_input, int& result, bool& exp_entered, ofstream& log_file);

int main()
{
	
	Infix ie;
	string user_input = "";
	int result = 0;
	bool exp_entered = false;

	ofstream log_file;
	log_file.open("log.txt");
	if (!log_file.is_open())
	{
		cerr << "Log.txt did not open, bye!" << endl;
	}

	
	while (true)
	{
		display(ie, user_input, result, exp_entered, log_file);
	}
	log_file.close();
}


void display(Infix& ie, string& user_input, int& result, bool& exp_entered, ofstream& log_file)
{



	int Selection(-1);
	cout << "*******select one of the following*******: \n\n";
	cout << "1:\tEnter an Infix Expression \n";
	cout << "2:\tExit\n";


	
	while (!(cin >> Selection))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		
		cout << endl << "Single digits only" << endl;
		cout << "Make another selection: ";
	}

	
	switch (Selection)
	{
		
	case 1:
		cout << endl << endl;
		cout << "Enter the Infix Expression: ";
		cin.ignore();
		getline(cin, user_input);
		log_file << "User entered " << user_input << endl;
		
		try {
			inspect Ins;
			user_input = Ins.Processinput(user_input);
			ie.clearData();
			result = ie.evaluate(user_input, log_file);
			exp_entered = true;
			cout << endl;
			cout << "Result is: " << result << endl;
			log_file << "Result is: " << result << endl;
		}

		
		catch (const std::exception &e)
		{
			std::cout << std::endl << std::endl;

			std::cout << e.what() << std::endl << std::endl;
			log_file << e.what() << endl;
			std::cout << "Calculation failed, try again!!!" << std::endl;
			std::cout << "Purging previous data" << std::endl;

			
			result = 0;
			exp_entered = false;
			user_input = "";
			ie.clearData();

		}

		cout << endl << endl;
		break;

	
		
	case 2:
		exit(0);
		log_file.close();

		
	default:
		cout << endl << endl;
		cout << "Invalid menu option" << endl;
		log_file << "Invalid menu option" << endl;
		break;
	}
}
