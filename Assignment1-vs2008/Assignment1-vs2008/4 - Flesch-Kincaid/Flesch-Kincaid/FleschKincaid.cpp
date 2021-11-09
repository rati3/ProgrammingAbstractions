#include <iostream>
#include <fstream>
#include <cctype>
#include "console.h"
#include "tokenscanner.h"
using namespace std;

/* Coefficients of Grade level */
const double C0 = -15.59;
const double C1 = 0.39;
const double C2 = 11.8;

/* number of words, syllables and sentences */
struct statistic {
	int words;
	int syllables;
	int sentences;
	statistic() { // default values
		words = 0; 
		syllables = 0;
		sentences = 0;
	}
};

/* Function prototypes */
void openFile(ifstream &file);
statistic getStatistic(ifstream &file);
int numberOfSyllables(string word);
bool is_syllable(char ch, bool isLastDigit);
double grade(statistic fileStat, double c0 = C0, double c1 = C1, double c2 = C2);

/* Main program */
int main() {
	ifstream file; // initilize file
	openFile(file); // open file
	statistic fileStat = getStatistic(file); // get statistic from file
	/* display statistic and grade */
	cout << "Words: " << fileStat.words << endl;
	cout << "Syllables: " << fileStat.syllables << endl;
	cout << "Sentences: " << fileStat.sentences << endl;
	cout << "Grade level: " << grade(fileStat) << endl;

	file.close(); // close file
	
    return 0;
}

void openFile(ifstream &file) {
	string fileName;
	while (true) {
		cout << "Enter file name: ";
		cin >> fileName;
		file.open(fileName.c_str());
		if (file.is_open())
			break;
		file.clear(); // clear file for next try
	}
}

statistic getStatistic(ifstream &file) {
	statistic result; // initialize struct
	/* initialize tokenScanner */
	TokenScanner scanner(file);
	scanner.ignoreWhitespace();
	scanner.addWordCharacters("'");
	scanner.addWordCharacters("-");
	/* get tokens and count statistics */
	bool lastWasEnding = false; // if last token was ? ! or .
	while(scanner.hasMoreTokens()) {
		string str = scanner.nextToken();	
		if (isalpha(str[0])){ // if token starts with alpabetic char
			result.words++;
			result.syllables += numberOfSyllables(str);
		}
		if (str == "." || str == "!" || str == "?") {
			if (!lastWasEnding) // do not double count  ... or ?!
				result.sentences++;
			lastWasEnding = true;
		} else {
			lastWasEnding = false;
		}
	}
	/* do not return value of 0 */ 
	if (result.words == 0) result.words++;
	if (result.sentences == 0) result.sentences++;
	if (result.syllables == 0) result.syllables++;
	return result;
}

int numberOfSyllables(string word) {
	int count = 0;
	bool lastIsSyllable = false;
	for (int i = 0; i < word.length(); i++) { 
		char ch = word[i];
		if (is_syllable(ch,i == word.length() - 1)) { // if ch is syllable
			if (!lastIsSyllable) // and last was not
				count++;
			lastIsSyllable = true;
		} else {
			lastIsSyllable = false;
		}
	}
	if (count == 0) // do not return value of 0
		return 1;
	return count;
}

bool is_syllable(char ch, bool isLastDigit) {
	switch (ch) {
		case 'A': case 'I': case 'O': case 'U': case 'Y':
		case 'a': case 'i': case 'o': case 'u': case 'y':
			return true;
		case 'E': case 'e':
			return !isLastDigit;
		default:
			return false;
	}
}

double grade(statistic fileStat, double c0 , double c1 , double c2) {
	return c0 + c1 * ((double)fileStat.words / fileStat.sentences) + c2 * ((double)fileStat.syllables / fileStat.words);
}
