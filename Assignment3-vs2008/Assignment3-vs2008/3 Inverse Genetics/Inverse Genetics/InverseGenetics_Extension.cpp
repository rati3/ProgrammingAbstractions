#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "vector.h"
#include "map.h"
#include "console.h"

using namespace std;


/* Loads the codon mapping table from a file. */
Map<string, char > load_CodonMap();

void fillNucleotidesVector(Vector<char> &nucleotides);
double probabilityOfMutation (string strand, Vector<char> &nucleotides, Map<string, char > &codons);
bool isSameProtein(string strand1, string strand2, Map<string, char > &codons, int length = 3);

int main1() {
    /* Load the codon map. */
	Map<string, char > codons = load_CodonMap();
	

	Vector<char> nucleotides;
	fillNucleotidesVector(nucleotides);

	string protein;
	cout << "Enter length by 9 protein using (A,O,G,U) letters: ";
	cin >> protein;

	cout << "Same proteins: " << endl;
	double probability = probabilityOfMutation(protein, nucleotides, codons);
	
	cout << "Probability of changing proteins is " << probability << endl;

	
    return 0;
}

void fillNucleotidesVector(Vector<char> &nucleotides) {
	nucleotides.add('A');
	nucleotides.add('C');
	nucleotides.add('G');
	nucleotides.add('U');
}

double probabilityOfMutation (string strand, Vector<char> &nucleotides, Map<string, char > &codons) {
	int count = 0;
	for (int i = 0; i < nucleotides.size(); i++) {
		for (int j = 0; j < strand.length(); j++) {
			if (nucleotides[i] == strand[j]) {
				count++;
				continue;
			}
			string newStrand = strand.substr(0,j) + nucleotides[i] + strand.substr(j+1);
			if (!isSameProtein(strand,newStrand,codons)) {
				count++;
			} else {
				cout << newStrand << ", ";
			}
		}
	}
	cout << endl;
	int numOfAllMutation = nucleotides.size() * strand.length();
	return (double) count / numOfAllMutation;

}

bool isSameProtein(string strand1, string strand2, Map<string, char > &codons, int length) {
	for (int i = 0; i < length; i++) 
		if (codons[strand1.substr(i*length,length)] != codons[strand2.substr(i*length,length)])
			return false;
	return true;
}


Map<string, char > load_CodonMap() {
    ifstream input("codons.txt");
    Map<string, char > result;

    /* The current codon / protein combination. */
    string codon;
    char protein;

    /* Continuously pull data from the file until all data has been
     * read.
     */
    while (input >> codon >> protein) {
        result[codon] += protein;
    }

    return result;
}
