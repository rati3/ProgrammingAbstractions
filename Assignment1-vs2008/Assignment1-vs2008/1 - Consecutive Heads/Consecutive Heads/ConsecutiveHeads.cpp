#include <iostream>
#include <time.h>
#include "console.h"

using namespace std;

    /* Constants  */
const int HEADS_IN_ROW_TO_STOP = 3;

void numberOfCoinFlips(int headsInRowToStop){
	int count = 0; // count of flips
	int headsInRow = 0; // count of heads in row
	srand(time(NULL));
	while (true) {
		count++;
		if (rand() % 2 == 1) { // if its head
			cout << "heads" << endl;
			headsInRow++;
			if (headsInRow == headsInRowToStop) { 
				cout << "It took " << count << " flips to get " << headsInRowToStop << " consecutive heads." << endl;
				break;
			}
		} else { // if its tails
			headsInRow = 0;
			cout << "tails" << endl;
		}
	}
}

int main() {
    numberOfCoinFlips(HEADS_IN_ROW_TO_STOP);
    return 0;
}
