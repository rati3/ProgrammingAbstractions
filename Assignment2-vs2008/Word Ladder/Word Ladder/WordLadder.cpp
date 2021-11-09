#include <iostream>
#include "vector.h"
#include "queue.h"
#include "lexicon.h"
#include "console.h"
#include "simpio.h"
using namespace std;

              /* Function prototypes */
/* get English word from user*/
string getWord(Lexicon &englishWords,string outputMessage);
/* get ladder of two words */
Vector<string> wordLadder(string startingWord, string destionation,Lexicon &englishWords);
/* print the ladder*/
void printLadder(Vector<string> &ladder);


int main() {
	// Initialize Lexicon of English words
	Lexicon englishWords("EnglishWords.dat");

	// get starting & ending words from user
	string starting = getWord(englishWords,"Enter The Starting Word: ");
	string destination = getWord(englishWords,"Enter The Destination Word: ");

	// checking
	if (starting.length() != destination.length()) {       // if words are not same length
		cout << "Words must be the same length." << endl; // do not start searching 
		return 0;
	}
	if (starting == destination) {                            // if words are the same
		cout << "The two words must be different." << endl;  // do not start searching
		return 0;
	}
	// display "Searching... "
	cout << "Searching... " << endl;
	// get word ladder
	Vector<string> ladder = wordLadder(starting,destination,englishWords);
	// display it
	printLadder(ladder);

    return 0;
}

string getWord(Lexicon &englishWords,string outputMessage){
	string word = getLine(outputMessage);
	while (true) {
		if (englishWords.contains(word))
			return word;
		word = getLine("Please enter English word - ");
	}

}
Vector<string> wordLadder(string startingWord, string destionation,Lexicon &englishWords) {
	Lexicon usedWords; // Initialize lexicon of used words

	Queue<Vector<string> > ladders;				   // Initialize queue for ladders
	Vector<string> CurrentLadder(1,startingWord); // Initalize starting ladder for queue
	ladders.enqueue(CurrentLadder);              // add starting ladder in queue

	string currentWord;
	while (!ladders.isEmpty()) {
		CurrentLadder = ladders.dequeue(); // get & remove ladder from queue
		currentWord = CurrentLadder.get(CurrentLadder.size() - 1); // get last word of ladder
		if (currentWord == destionation) // if the word is destination
			return CurrentLadder;
		for (int ch = 'a'; ch <= 'z'; ch++) { 
			for (int i = 0; i < currentWord.length(); i++) {
				string newWord = currentWord; // crete new word that
				newWord[i] = char(ch);       // differs by one letter
				if (englishWords.contains(newWord) && !usedWords.contains(newWord)) { // if new word is valid & not used 
					Vector<string> newLadder = CurrentLadder;  // copy current ladder
					newLadder.add(newWord);                   // add new word in new ladder
					ladders.enqueue(newLadder);              // add new ladder at the end of queue
					usedWords.add(newWord);                 // and save new word as used
				}
			}
		}
	}
	CurrentLadder.clear();  // if you are here no ladder is found
	return CurrentLadder;  // so return empty vector

}

void printLadder(Vector<string> &ladder) {
	if(ladder.size() == 0) { // if ladder is empty
		cout << "No word ladder could be found." << endl;
		return;
	}
	for (int i = 0; i < ladder.size() - 1; i++)
			cout << ladder.get(i) << " -> ";
	cout << ladder.get(ladder.size() - 1) << endl;
}
