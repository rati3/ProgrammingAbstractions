#include <iostream>
#include <time.h>
#include "console.h"

using namespace std;

    /* Constants  */
string SEQUENCE_1 = "HHHHH";
string SEQUENCE_2 = "HTHTT";
const int N_FLIP = 10000; // total number of flips

void numberOfSecuences(int totalFilps = N_FLIP, string seq_1 = SEQUENCE_1, string seq_2 = SEQUENCE_2){
	int count_1 = 0;  // count of sequence one
	int count_2 = 0; // count of sequence two
	srand(time(NULL));
	string lastFlips;
	for (int i = 0; i < totalFilps;	i++) {
		if (rand() % 2 == 1) { // if its head
			lastFlips += "H";
		} else { // if its tails
			lastFlips += "T";
		}
		if (lastFlips.length() > seq_1.length()) // keep length of string e.g. 5
				lastFlips = lastFlips.substr(1,lastFlips.length()-1);
		if(lastFlips == seq_1) count_1++;
		if(lastFlips == seq_2) count_2++;
	}
	cout << count_1 << "   " << count_2 << endl;
}

int main1() {
	numberOfSecuences();
    return 0;
}
