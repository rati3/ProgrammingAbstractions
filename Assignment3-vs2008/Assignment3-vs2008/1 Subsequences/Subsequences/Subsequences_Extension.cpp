#include <iostream>
#include <string>
#include "set.h"
#include "console.h"

using namespace std;

/* Method finds smallest string that each string from set is subSeq of the string */
void smallestSubsequence(Set<string> &words, string current, string &smallest);

bool is_subsequence(string text, string subsequence);

int main1() {
	Set<string> test;
	test.add("ratiegg");
	test.add("egg");
	test.add("tiger");
	test.add("egzotic");
	string current,smallest;
	smallestSubsequence(test, current, smallest);
	cout << smallest << endl;
	
    return 0;
}

void smallestSubsequence(Set<string> &words, string current, string &smallest) {
	if (words.size() == 0) { // Base Case : set is empty
		          /* first check or current is smaller than smallest */
		if (smallest.length() == 0 || smallest.length() > current.length()) 
			smallest = current;
		return;
	}
	
	/* check every sequence of words */
	foreach (string currentWord in words) {	
		words.remove(currentWord);
		int length = 1;
		for ( ; length <= currentWord.length(); length++) {              // check each substr from 0
			if (!is_subsequence(current,currentWord.substr(0,length)))  // if it is subseq of current
				break;
		}

		string newCurrent = current + currentWord.substr(length - 1); // add the part which is not subseq
		smallestSubsequence(words ,newCurrent,smallest);              // check the rest of

		words.add(currentWord);  // undo
	}
}


bool is_subsequence(string text, string subsequence) {
	/* Base Case : if subseq is empty return true */
	if (subsequence.length() == 0) return true; 
	/* Base Case : if text is empty return false */
	if (text.length() == 0) return false;

	/* otherwise compare fisrt chars of text & subSeq */
	string newText = text.substr(1, text.length() - 1);
	string newSubSeq = subsequence.substr(1, subsequence.length() - 1);
	/* if they are equal, erase that chars & check the rest of */
	if (subsequence[0] == text[0])
		return is_subsequence(newText, newSubSeq);
	/* if not equal, check erase first char of text & check */
	return is_subsequence(newText, subsequence);
}

