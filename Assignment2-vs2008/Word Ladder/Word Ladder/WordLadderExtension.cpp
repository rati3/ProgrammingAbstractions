#include <iostream>
#include "vector.h"
#include "queue.h"
#include "lexicon.h"
#include "console.h"
#include "simpio.h"
using namespace std;

              /* Function prototypes */

/* get English word from user*/
string GetWord(Lexicon &englishWords,string outputMessage);

/* Method swaps strings */
void swapStrings(string &str1, string &str2);

/* Method returns ladder of two words */
Vector<string> getWordLadder(string startingWord, string destionation,Lexicon &englishWords);

/* If new word is English word and not used, add the word in vector and the vector add in Queue */
void addIfWordIsValid(Queue<Vector<string> > &ladders, Vector<string> CurrentLadder,Lexicon &englishWords,Lexicon &usedWords,string newWord);

/* Method creates new word by adding one character & if it's valid adds in queue */
void addLongerWordsInQueue(Queue<Vector<string> > &ladders, Vector<string> CurrentLadder, Lexicon &englishWords,Lexicon &usedWords);

/* Method adds character in word at index */
string updateWord(string word,char ch, int index);

/* print the ladder*/
void PrintLadder(Vector<string> &ladder);


int main7() {
	// Initialize Lexicon of English words
	Lexicon englishWords("EnglishWords.dat");
	
	// get starting & ending words from user
	string starting = GetWord(englishWords,"Enter The Starting Word: ");
	string destination = GetWord(englishWords,"Enter The Destination Word: ");

	// checking
	if (starting == destination) {                            // if words are the same
		cout << "The two words must be different." << endl;  // do not start searching
		return 0;
	}
	if (starting.length() > destination.length()) 
		swapStrings(starting,destination);	

	// display "Searching... "
	cout << "Searching... " << endl;
	
	Vector<string> ladder;
	ladder = getWordLadder(starting,destination,englishWords);
	PrintLadder(ladder);

    return 0;
}

string GetWord(Lexicon &englishWords,string outputMessage){
	string word = getLine(outputMessage);
	while (true) {
		if (englishWords.contains(word))
			return word;
		word = getLine("Please enter English word - ");
	}

}

void swapStrings(string &str1, string &str2) {
	string temp = str1;
	str1 = str2;
	str2 = temp;
}

Vector<string> getWordLadder(string startingWord, string destionation,Lexicon &englishWords) {
	Lexicon usedWords;

	Queue<Vector<string> > ladders;				   // Initialize queue for ladders
	Vector<string> CurrentLadder(1,startingWord); // Initalize starting ladder for queue
	ladders.enqueue(CurrentLadder);              // add starting ladder in queue

	string currentWord;
	while (!ladders.isEmpty()) {
		CurrentLadder = ladders.dequeue(); // get & remove ladder from queue
		currentWord = CurrentLadder.get(CurrentLadder.size() - 1); // get last word of ladder
		if (currentWord == destionation) // if the word is destination
			return CurrentLadder;
		
		if (currentWord.length() < destionation.length())
			addLongerWordsInQueue(ladders,CurrentLadder,englishWords,usedWords);

		for (int ch = 'a'; ch <= 'z'; ch++) { 
			for (int i = 0; i < currentWord.length(); i++) {
				string newWord = currentWord; // crete new word that
				newWord[i] = char(ch);       // differs by one letter
				addIfWordIsValid(ladders,CurrentLadder,englishWords,usedWords,newWord);
			}
		}
	}
	CurrentLadder.clear();  // if you are here no ladder is found
	return CurrentLadder;  // so return empty vector

}

void addLongerWordsInQueue(Queue<Vector<string> > &ladders, Vector<string> CurrentLadder, Lexicon &englishWords,Lexicon &usedWords){
	string updatedWord;
	string currentWord = CurrentLadder.get(CurrentLadder.size() - 1);
	for (int j = 0; j <= currentWord.length(); j++) { // j is index of addition char
		for (int ch = 'a'; ch <= 'z'; ch++) {
			updatedWord = updateWord(currentWord,ch,j);
			addIfWordIsValid(ladders,CurrentLadder,englishWords,usedWords,updatedWord);			
		}
	}
}
string updateWord(string word,char ch, int index) {
	string updatedWord;
	updatedWord = word.substr(0,index);
	updatedWord += ch;
	updatedWord += word.substr(index,word.length() - index);
	return updatedWord;
}

void addIfWordIsValid(Queue<Vector<string> > &ladders, Vector<string> CurrentLadder,Lexicon &englishWords,Lexicon &usedWords,string newWord) {
	if (englishWords.contains(newWord) && !usedWords.contains(newWord)) { // if new word is valid & not used 
		CurrentLadder.add(newWord);                                      // add new word in ladder
		ladders.enqueue(CurrentLadder);                                 // add ladder at the end of queue
		usedWords.add(newWord);                                        // and save new word as used
	}                                                              
}

void PrintLadder(Vector<string> &ladder) {
	if(ladder.size() == 0) { // if ladder is empty
		cout << "No word ladder could be found." << endl;
		return;
	}
	for (int i = 0; i < ladder.size() - 1; i++)
			cout << ladder.get(i) << " -> ";
	cout << ladder.get(ladder.size() - 1) << endl;
}


				