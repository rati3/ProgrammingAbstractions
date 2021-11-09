#include <iostream>
#include <string>
#include "strlib.h"
#include "set.h"
#include "vector.h"
#include "simpio.h"
#include "tokenscanner.h"
#include "console.h"
using namespace std;

/* special stopping word */
const int STOP = -1;

int bestOfferUniversalCoverage(Vector<int> & populations, int MaxNumPeople, int index = 0);

int main1() {
	Vector<int> populations;
	int count = 0;
	int num;
	while (true) {
		count++;
		cout << "Enter population of city-" << count << ": ";
		cin >> num;
		if (num == STOP) break;
		populations.add(num);
	}
	
	int maxNumPeople;
	cout << "Enter maximum number of people could be provided: ";
	cin >> maxNumPeople;

	cout << "Best offer: " << bestOfferUniversalCoverage(populations, maxNumPeople) << endl;

    return 0;
}



int bestOfferUniversalCoverage(Vector<int> & populations, int MaxNumPeople, int index) {
	/* Base Case: check all city */
	if (index == populations.size()) return 0;

	/* Base Case: population of current city is more than max num of people */
	int currentCity = populations[index]; 
	if (MaxNumPeople < currentCity) return 0;

	int useCurrentCity = currentCity + bestOfferUniversalCoverage(populations, MaxNumPeople - currentCity, index + 1);

	int noCurrentCity = bestOfferUniversalCoverage(populations, MaxNumPeople, index + 1);

	/* return best offer */
	return max(useCurrentCity,noCurrentCity);

	
}
                               
