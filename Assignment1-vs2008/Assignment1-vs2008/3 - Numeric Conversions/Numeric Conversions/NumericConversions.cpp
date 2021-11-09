/*
 * File: NumericConversions.cpp
 * ---------------------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the Numeric Conversions problem.
 * [TODO: rewrite the documentation]
 */

#include <iostream>
#include <string>
#include "console.h"
using namespace std;

/* Function prototypes */

string intToString(int n);
int stringToInt(string str);
bool isInteger(string str);
void getInteger(string &str);

/* Main program */

int main() {
	string str;  
	getInteger(str);                   // get integer from user
	int n = stringToInt(str);         // cast to int
	cout << n << endl;               // display int
	cout << intToString(n) << endl; // cast to string and display
    return 0;
}

void getInteger(string &str) {
	while (true) { // enter a number until it's valid
		cout << "Enter a number: ";
	    cin >> str;
		if (isInteger(str)) 
			break;
	}
}

bool isInteger(string str) {
	if (str.length() == 0) // empty string
		return false;
	/* check first char, if it is '-' check second too */
	if ((str[0] < '1' || str[0] > '9') && (str[0] != '-' || str[1] < '1' || str[1] > '9') && str != "0")  
		return false; // examples 01 , r4 or -03, -g5
	for (int i = 1; i < str.length(); i++) {
		if (str[i] < '0' || str[i] > '9')
			return false;
	}
	return true;
		
}


int stringToInt(string str) {
	if (str[0] == '-') // if number is negative 
		return -(stringToInt(str.substr(1,str.length() - 1)));

	if (str.length() == 1) 
		return str[0] - '0';

	return (str[str.length() - 1] - '0') + 10 * stringToInt(str.substr(0,str.length() - 1)); // last digit plus 10 * remaining
	
}

string intToString(int n) {
	if (n < 0) 
		return "-" + intToString(-n); 

	if (n / 10 == 0) // one digit
		return string() + char('0' + n);

	return intToString(n/10) + char('0' + (n % 10)); 
}
