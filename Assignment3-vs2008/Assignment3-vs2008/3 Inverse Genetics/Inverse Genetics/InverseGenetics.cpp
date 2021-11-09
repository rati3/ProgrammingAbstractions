#include <iostream>
#include <string>
#include <fstream>
#include "set.h"
#include "map.h"
#include "console.h"
using namespace std;

/* Given a protein and a map from amino acid codes to the codons for
 * that code, lists all possible RNA strands that could generate
 * that protein
 */
void listAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons);

void printAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons, string current);

/* Loads the codon mapping table from a file. */
Map<char, Set<string> > loadCodonMap();

int main() {
    /* Load the codon map. */
    Map<char, Set<string> > codons = loadCodonMap();

	string protein;
	cout << "Enter protein: ";
	cin >> protein;

	listAllRNAStrandsFor(protein,codons);
    
    return 0;
}


void listAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons) {
	string current;
	printAllRNAStrandsFor(protein, codons, current);
}

void printAllRNAStrandsFor(string protein, Map<char, Set<string> >& codons, string current) {
	if (protein.length() == 0) {   // if there is no more amino acid 
		cout << current << endl;  // display current 
		return;
	}

	char aminoAcid = protein[0];                                   // current amino acid
	string allButFirst = protein.substr(1,protein.length() - 1);  // protein except first amino acid
	foreach (string codon in codons[aminoAcid]) {
		string newCurrent = current + codon;                     // create new current with every codon of aminoAcid
		printAllRNAStrandsFor(allButFirst,codons,newCurrent);   
	}
}


Map<char, Set<string> > loadCodonMap() {
    ifstream input("codons.txt");
    Map<char, Set<string> > result;

    /* The current codon / protein combination. */
    string codon;
    char protein;

    /* Continuously pull data from the file until all data has been
     * read.
     */
    while (input >> codon >> protein) {
        result[protein] += codon;
    }

    return result;
}
