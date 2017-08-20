// ReadingCSV.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

enum MenuOption
{
	OPEN = 1,
	SAVE = 2,
	EXIT = 3
};

enum Month
{
	JANUARY = 1,
	FEBRUARY = 2,
	MARCH = 3,
	APRIL = 4,
	MAY = 5,
	JUNE = 6,
	JULY = 7,
	AUGUST = 8,
	SEPTEMBER = 9,
	OCTOBER = 10,
	NOVEMBER = 11,
	DECEMBER = 12
};

string dequote(string s);
MenuOption menu();
void open();
void save();
bool exists(const string PATH);
void read_raw(vector<string> raw);
void display_averages();
string month_name(Month month);
string print_log();


map<Month, vector<double > > readings; // will delete itself when it's score is exited (the program itself)

int main()
{
	cout << "Raymond Dinkin | 200257283\n\n" << endl;

	bool running = true;
	MenuOption selection;

	while (running)
	{
		selection = menu();

		if (selection == OPEN)
			open();

		if (selection == SAVE)
			save();

		if (selection == EXIT)
			running = false;
	}

	system("PAUSE");
	return 0;
}

// get the text form of the csv
string print_log()
{
	string text = "";

	for (size_t i = 1; i <= 12; i++)
	{
		Month month = static_cast<Month>(i);
		text += month_name(month) + ",";
		for (size_t i = 0; i < readings[month].size(); i++)
		{
			text += to_string(readings[month][i]);

			if (i != readings[month].size() - 1)
				text += ",";
		}

		text += "\n";
	}

	cout << text;
	return text;
}

// save the data to a csv file
void save()
{
	if (print_log() != "")
	{
		string text = print_log();
		bool valid = false;
		string PATH;

		while (!valid)
		{
			cout << "Enter a filename: ";
			cin >> PATH;

			if (!exists(PATH) && PATH.find('.') == string::npos && PATH.find(' ') == string::npos && PATH.find(',') == string::npos)
				valid = true;
		}

		PATH += ".csv";
		ofstream file(PATH);

		file << print_log();

		file.close();
	}
}

// convert an enum type of month to it's corresponding text representation
string month_name(Month month)
{
	if (month == JANUARY)
		return "JANUARY";

	if (month == FEBRUARY)
		return "FEBRUARY";

	if (month == MARCH)
		return "MARCH";

	if (month == APRIL)
		return "APRIL";

	if (month == MAY)
		return "MAY";

	if (month == JUNE)
		return "JUNE";

	if (month == JULY)
		return "JULY";

	if (month == AUGUST)
		return "AUGUST";

	if (month == SEPTEMBER)
		return "SEPTEMBER";

	if (month == OCTOBER)
		return "OCTOBER";

	if (month == NOVEMBER)
		return "NOVEMBER";

	return "DECEMBER";
}

// start parsing the a menu, items are stored as an enum
MenuOption menu()
{
	bool valid = false;
	int selection;

	while (!valid)
	{
		cout << "1.	Open a Climate File to Process" << endl;
		cout << "2.	Save the Climate Data" << endl;
		cout << "3.	Exit" << endl;
		cout << "Please enter a menu option: ";
		
		cin >> selection;

		if (selection == 1 || selection == 2 || selection == 3)
			valid = true; 

		cout << "\n";
			
	}
	return static_cast<MenuOption>(selection);
}

// see if a file already exists
bool exists(const string PATH)
{
	ifstream file(PATH);
	bool is_good = file.good();
	file.close();
	return is_good;
}

// open a file and get it's data, after write the data to the console
void open()
{
	string PATH;
	bool good = false;

	while (!good)
	{
		cout << "Enter a file name to open:  ";
		cin >> PATH;

		if (exists(PATH))
		{
			good = true;
		}
		else
		{
			cout << "please enter a valid path" << endl;
		}
	}

	cout << "\n Attempting to open \"" << PATH << "\"\n" << endl;



	// read file here
	ifstream file(PATH);

	vector<string> raw;

	while (file.good())
	{
		string line;
		getline(file, line);
		raw.push_back(dequote(line));
	}

	read_raw(raw);
	display_averages();


	file.close();
}

// reads the results map and displays it as a function of averages
void display_averages()
{
	for (size_t i = 1; i <= 12; i++)
	{
		double average = 0;
		Month month = static_cast<Month>(i);
		cout << month_name(month) << ": ";
		for (size_t j = 0; j < readings[month].size(); j++)
		{
			// cout << readings[month][j] << " "; uncomment to show daily output
			average += readings[month][j];
		}
		cout << average/readings[month].size() << "c of " << readings[month].size() << " days measuered\n";
	}

	cout << "\n\n";
}

// takes the raw line, read input (without the quotes) and translates the useful data into the results map to be stored dynamically
void read_raw(vector<string> raw)
{
	for (size_t i = 0; i < raw.size(); i++)
	{
		vector<string> tokens;
		string line = raw[i];

		stringstream ss(line);

		while (ss.good())
		{
			string token;
			getline(ss, token, ',');
			tokens.push_back(token);
		}

		ss.clear();

		if (tokens.size() > 5 && tokens.at(2) != "" && tokens.at(5) != "" && tokens.at(2) != "M" && tokens.at(5) != "M")
		{
			int month = stoi(tokens.at(2));
			double max_temp = atof(tokens.at(5).c_str());
			readings[static_cast<Month >(month)].push_back(max_temp);
		}
	}
}

// removes all quotes from a string
string dequote(string s)
{
	s.erase(remove_if(
		s.begin(), s.end(),
		[](const char& x) {
		return x == '\"';
	}), s.end());
	
	return s;
}