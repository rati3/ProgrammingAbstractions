#include <iostream>
#include <fstream>
#include "console.h"
#include "vector.h"
#include "simpio.h"
#include "random.h"
#include "map.h"
#include "tokenscanner.h"

using namespace std;
/* Number of symbols to random write */
const int num_of_word = 500;
/* Display the error when number of symbols in file is less than order */
const string too_small_file_error = "The File is too small to create text."; 

 

              /* Function prototypes */

/* Method gets filename from user until file be opened */
void OpenFile(ifstream &file);

/* Method gets valid integer between [1 - 10] area */
int GetOrder();

/* Method returns string of first characters (length of order) from file */ 
string getFirstWords(TokenScanner &scanner, int order);

/* Method saves information of file in map. It's key is string of words with number of order & value is Vector of following words.
	Method returns the most commonly occurring words */
string getStatistic(Map<string, Vector<string> > &map, TokenScanner &scanner, string currentWords);

/* Method saves following word of current words. Method returns size of appropriate vector */ 
int saveNextWord(string word, string words, Map<string, Vector<string> > &map);

/* If current is more than maximum overwrite most values */
void checking(int currentOccuring, string currentWords, int &maxOccuring, string &mostRepeatedWords);

/* Method removes first word and adds word at the end */
string getNewCurrentWords(string word, string currentWords);

/* Methods print random text according to the information of map. text starts with mostUsedWords */
void PrintRandomText(Map<string, Vector<string> > &map, string mostUsedWords, int order);

/* Method changes current words according to the information of map. If there is no info about currentWords returns false, otherwise true */
bool getNextWord(string &nextWord, Map<string, Vector<string> > &map, string currentWords);

int main7() {
	ifstream file; // Initialize input file
	OpenFile(file);
	TokenScanner scanner(file);
	scanner.ignoreWhitespace();
	int order = GetOrder();

	string firstWords = getFirstWords(scanner,order); 
	if (firstWords == too_small_file_error) {  // if words in file is less than order      
		cout << too_small_file_error << endl;
		return 0;                                     
	}
	cout << "Processing file..." << endl;

	Map<string, Vector<string> > map; // Initialize map
	string mostUsedWords = getStatistic(map,scanner,firstWords);
	PrintRandomText(map,mostUsedWords,order);
	
    return 0;
}

void OpenFile(ifstream &file) {
	string fileName;
	while (true) {	
		fileName = getLine("Enter the source text: ");
		file.open(fileName.c_str());
		if (file.is_open())
			break;
		file.clear(); // clear file for next try
		cout << "Unable to open the file.  Try again." << endl;
	}
}

int GetOrder() {
	int order;
	while (true) {
		order = getInteger("Enter the Markov order [1-10]: ");
		if (order >= 1 && order <= 10) // if order is in [1-10] area
			return order;
		cout << "That value is out of range." << endl;	
	}
	return -1;
}


string getFirstWords(TokenScanner &scanner, int order) {
	string firstWords;
	for (int i = 0; i < order; i++) {
		if (!scanner.hasMoreTokens())  // if words in file is less than order
			return too_small_file_error;
		firstWords += scanner.nextToken() + " ";
	}
	return firstWords.substr(0,firstWords.length() - 1); // remove last space 
}

string getStatistic(Map<string, Vector<string> > &map, TokenScanner &scanner, string currentWords) {
	string mostRepeatedWords;
	int maxOccuring = 0;
	int currentOccuring;
	string nextWord;
	
	while (scanner.hasMoreTokens()) {
		nextWord = scanner.nextToken();
		currentOccuring = saveNextWord(nextWord,currentWords,map);
		checking(currentOccuring,currentWords,maxOccuring,mostRepeatedWords);
		currentWords = getNewCurrentWords(nextWord,currentWords);
	}
	currentOccuring = map[currentWords].size() + 1;
	checking(currentOccuring,currentWords,maxOccuring,mostRepeatedWords);
	return mostRepeatedWords;
}
int saveNextWord(string word, string words, Map<string, Vector<string> > &map) {
	map[words].add(word);
	return map[words].size();
}

void checking(int currentOccuring, string currentWords, int &maxOccuring, string &mostRepeatedWords) {
	if (currentOccuring > maxOccuring) {
		mostRepeatedWords = currentWords;
		maxOccuring = currentOccuring;
	}
}

string getNewCurrentWords(string word, string currentWords) {
	string newCurrent;
	TokenScanner scanner(currentWords);
	scanner.ignoreWhitespace();
	scanner.nextToken(); // first word
	while(scanner.hasMoreTokens())
		newCurrent += scanner.nextToken() + " ";  // save next words
	newCurrent += word;                          // and new word
	return newCurrent;
} 

void PrintRandomText(Map<string, Vector<string> > &map, string mostUsedWords,int order) {
	string currentWords = mostUsedWords;
	string nextWord;
	cout << currentWords;
	for (int i = 0; i < num_of_word - order; i++) {
		if (getNextWord(nextWord,map,currentWords)) {
			cout << " " << nextWord; // print next word
			currentWords = getNewCurrentWords(nextWord,currentWords); // change current words
		} else {
			cout << endl;
			cout << "Program stop working early." << endl;
			return;  // stop writing early
		}
	}
	cout << endl;
}

bool getNextWord(string &nextWord, Map<string, Vector<string> > &map, string currentWords) {
	if (map[currentWords].size() == 0) // if there is no info
		return false;
	int randomIndex = randomInteger(0,map[currentWords].size() - 1);
	nextWord = map[currentWords][randomIndex]; // get random word
	return true;
}
