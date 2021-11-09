#include <iostream>
#include <string>
#include "console.h"
using namespace std;

bool isSubsequence(string text, string subsequence);

int main() {
	string text, subSeq;
	cout << "Enter text: ";
	cin >> text;
	cout << "Enter subSeq: ";
	cin >> subSeq;
	
	if (isSubsequence(text, subSeq)) {
		cout << "Is Subsequence" << endl;
	} else {
		cout << "Is not Subsequence" << endl;
	}
    return 0;
}

bool isSubsequence(string text, string subsequence) {
	/* Base Case : if subseq is empty return true */
	if (subsequence.length() == 0) return true; 
	/* Base Case : if text is empty return false */
	if (text.length() == 0) return false;

	/* otherwise compare fisrt chars of text & subSeq */
	string newText = text.substr(1, text.length() - 1);
	string newSubSeq = subsequence.substr(1, subsequence.length() - 1);
	/* if they are equal, erase that chars & check the rest of */
	if (subsequence[0] == text[0])
		return isSubsequence(newText, newSubSeq);
	/* if not equal, check erase first char of text & check */
	return isSubsequence(newText, subsequence);
}
