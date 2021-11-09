#include <iostream>
#include <fstream>
#include "console.h"
#include "map.h"
#include "vector.h"
#include "simpio.h"
#include "random.h"

using namespace std;

/* Number of symbols to random write */
const int NUM_OF_SYMBOL = 2000;
/* Display the error when number of symbols in file is less than order */
const string TOO_SMALL_FILE_ERROR = "The File is too small to create text."; 

              /* Function prototypes */

/* Method gets filename from user until file be opened */
void openFile(ifstream &file);

/* Method gets valid integer between [1 - 10] area */
int getOrder();

/* Method saves information of file in map. It's key is seed with length of order & value is Vector of following symbols.
	Method returns the most commonly occurring sequence */
string getStatistic(Map<string,Vector<char> > &map, ifstream &file, string sequence);

/* Method returns string of first characters (length of order) from file */ 
string getFirstSequence(ifstream &file, int order);

/* Method saves following symbol of current sequence. Method returns size of appropriate vector */
int saveFollowingSymbol(Map<string,Vector<char> > &map, string &sequence, char ch);

/* If current is more than maximum overwrite most values */
void checkIfCurrentIsMost(int &currentRepeated,int &mostRepeated, string &sequence, string &mostRepeatedSeq);

/* Methods print random text according to the information of map. text starts with first seed */
void printRandomText(Map<string,Vector<char> > &map, string mostUsedSeed,int order, int numOfSymbol = NUM_OF_SYMBOL);

/* Method changes current seed according to the information of map. If there is no info about currentSeed returns false, otherwise true */
bool getNextSeed(Map<string,Vector<char> > &map, string &currentSeed);

int main() {
	ifstream file; // Initialize input file
	openFile(file);
	int order = getOrder();

	Map<string,Vector<char> > map; // Initialize map

	string firstSequence = getFirstSequence(file,order); // get first sequence of file
	if (firstSequence == TOO_SMALL_FILE_ERROR) {        // if it's small file error 
		cout << TOO_SMALL_FILE_ERROR << endl;          // display it 
		return 0;                                     // and break
	}
	cout << "Processing file..." << endl;

	string mostUsedSeed; // the most commonly occurring sequence 
	mostUsedSeed = getStatistic(map,file,firstSequence); // save information in map and get mostUsedSeed
	
	printRandomText(map,mostUsedSeed,order);	
    return 0;
}

void openFile(ifstream &file) {
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

int getOrder() {
	int order;
	while (true) {
		order = getInteger("Enter the Markov order [1-10]: ");
		if (order >= 1 && order <= 10) // if order is in [1-10] area
			return order;
		cout << "That value is out of range." << endl;	
	}
	return -1;
}


string getStatistic(Map<string,Vector<char> > &map, ifstream &file, string sequence) {
	int currentRepeated;                           // number of current appearing
	int mostRepeated = 0;                         // max of appearing
	string mostRepeatedSeq;                      // max appearing sequence
	char ch; 
	while (file.get(ch)) { // while exists next char
		currentRepeated = saveFollowingSymbol(map,sequence,ch);
		checkIfCurrentIsMost(currentRepeated,mostRepeated,sequence,mostRepeatedSeq);
		sequence = sequence.substr(1,sequence.length() - 1) + ch; // get next sequence
	}
	currentRepeated = map[sequence].size() + 1; // save & check last sequence
	checkIfCurrentIsMost(currentRepeated,mostRepeated,sequence,mostRepeatedSeq);
	return mostRepeatedSeq;
}

string getFirstSequence(ifstream &file, int order) {
	string seq;
	char ch;
	for (int i = 0; i < order ; i++) {
		if (!file.get(ch)) // if symbols in file is less than order
			return TOO_SMALL_FILE_ERROR;
		seq += ch;
	}
	return seq;
}

int saveFollowingSymbol(Map<string,Vector<char> > &map, string &sequence, char ch) {
	map[sequence].add(ch); // save following symbol 
	return map[sequence].size();
	
}

void checkIfCurrentIsMost(int &currentRepeated,int &mostRepeated, string &sequence, string &mostRepeatedSeq) {
	if (currentRepeated > mostRepeated) { // if current is more than max, overwrite mostValues
			mostRepeatedSeq = sequence;
			mostRepeated = currentRepeated;
		}
}

void printRandomText(Map<string,Vector<char> > &map, string mostUsedSeed,int order ,int numOfSymbol) {
	string currentSeed = mostUsedSeed; // start text with
	cout << currentSeed;              // most used seed
	for (int i = 0; i < numOfSymbol - order; i++) {
		if (getNextSeed(map, currentSeed)) {                 // if there is info about current seed
			cout << currentSeed[currentSeed.length() - 1];  // write next seed
		} else {
			cout << endl;
			cout << "Program stop working early." << endl;
			return;  // stop writing early
		}
	}
	cout << endl;	
}

bool getNextSeed(Map<string,Vector<char> > &map, string &currentSeed) {
	if (map[currentSeed].size() == 0) // if there is not info about current seed
		return false;
	int randomIndex = randomInteger(0,map[currentSeed].size() - 1);                // get random index 
	char nextSymbol = map[currentSeed][randomIndex];                              // for next symbol
	currentSeed = currentSeed.substr(1,currentSeed.length() - 1) + nextSymbol;   // and create new seed
	return true;
}


