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
const string STOP = "stop";

/* Method gets line of cities & saves them in Set */
bool getCities(Set<string> &cities, string message = "Enter cities, separated by spaces: ");
/* Method gets locations from user */ 
void getLocations(Vector<Set<string> > &locations);

/*Given a set of cities, a list of what cities various hospitals can
 * cover, and a number of hospitals, returns whether or not it's
 * possible to provide coverage to all cities with the given number of
 * hospitals.  If so, one specific way to do this is handed back in the
 * result parameter.
 */
bool canOfferUniversalCoverage(Set<string>& cities,
                               Vector< Set<string> >& locations,
                               int numHospitals,
                               Vector< Set<string> >& result);

int main() {
	Set <string> cities;
	Vector<Set<string> > locations;

	getCities(cities);
	getLocations(locations);
	int maxHospitals = getInteger("Enter maximum number of hospitals: ");

	Vector <Set<string> > result;
	if (canOfferUniversalCoverage(cities, locations, maxHospitals, result)) { 
		cout << "Universal offer:" << endl;
		foreach (Set<string> set in result) { // display result
			foreach (string str in set)
				cout << str << " ";
			cout << endl;
		}

	} else {
		cout << "can not offer universal converage." << endl;
	}
    return 0;
}

bool getCities(Set<string> &cities, string message) {
	string line = getLine(message);
	if (line == STOP) return false;
	TokenScanner scanner(line);
	scanner.ignoreWhitespace();
	while(scanner.hasMoreTokens())
		cities.add(scanner.nextToken());
	return true;
}

void getLocations(Vector<Set<string> > &locations) {
	int count = 0;
	while (true) {
		count++;
		Set<string> cities;
		string message = (string)"Enter cities, hospital-" + integerToString(count) + " could service or \"stop\" to stop: ";
		if (!getCities(cities, message)) break;
		locations.add(cities);
	}

}


bool canOfferUniversalCoverage(Set<string>& cities, Vector< Set<string> >& locations, int numHospitals, Vector< Set<string> >& result) {
	if (cities.size() == 0) // if there is no more cities to converage, it's done
		return true;
	if (locations.size() == 0 || numHospitals == 0) // if there is no more hospital to offer
		return false;
	// otherwise

	Set<string> currentLocations = locations[0]; // current hospital locations
	locations.remove(0);                        // remove current locations from "remaining" locations
  
	// do not use current hospital
	if (canOfferUniversalCoverage(cities,locations,numHospitals,result))
		return true;

	result.add(currentLocations); // use current hospital
	if (canOfferUniversalCoverage(cities - currentLocations,locations,numHospitals - 1, result))
		return true;

	/* if neither way returns true, there is no other way so UNDO & return false */
	result.remove(result.size() - 1); // remove current locations from result
	locations.add(currentLocations); // add current locations in "remaining" locations
	
	return false;
	
		
	
}
                               
