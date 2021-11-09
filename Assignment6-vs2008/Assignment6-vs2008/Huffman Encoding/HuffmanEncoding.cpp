#include "HuffmanEncoding.h"

Map<ext_char, int> getFrequencyTable(istream& file) {
	Map<ext_char, int> result;
	
	char currentChar;              // while has more char
	while (file.get(currentChar)) // save it
		result[currentChar]++;   // count++
	
	result[PSEUDO_EOF]++;      
	
	return result;	
}

/* Method creates new node with ext_char and weight */
Node* getNewNode(ext_char character, int weight) {
	Node* newNode = new Node;
	newNode -> one = newNode -> zero = NULL; 
	newNode -> character = character;
	newNode -> weight = weight;

	return newNode;
}

/* Method saves nodes in priority queue by its frequencies from Map */
void saveInPriority(PriorityQueue<Node *> &priorityTree, Map<ext_char, int>& frequencies) {
	foreach (ext_char currentChar in frequencies) {
		Node* newNode = getNewNode(currentChar, frequencies[currentChar]);
		priorityTree.enqueue(newNode, newNode -> weight);
	}
}

/* Method returns merged Node of first and second */
Node* merge(Node* first, Node* second) {
	int weightSum = first -> weight + second -> weight; 
	Node * newNode = getNewNode(NOT_A_CHAR, weightSum);
	newNode -> zero = first;
	newNode -> one = second;
	
	return newNode;
}

/* Method dequeues two min, merged them and enqueue it */
void unionTwoMin(PriorityQueue<Node *> &priorityTree) {
	Node* firstMin = priorityTree.dequeue();
	Node* secondMin = priorityTree.dequeue();
	Node* merged = merge(firstMin, secondMin);
	priorityTree.enqueue(merged,merged -> weight);

}

/* Method builds a tree and saves top of it in result */
void getTopNode(Node* &result, PriorityQueue<Node *> &priorityTree) {
	while (priorityTree.size() != 1) {
		unionTwoMin(priorityTree);
	}
	result = priorityTree.dequeue();
}


Node* buildEncodingTree(Map<ext_char, int>& frequencies) {
	PriorityQueue<Node *> priorityTree;
	Node * result = NULL;
	saveInPriority(priorityTree, frequencies);
	getTopNode(result, priorityTree);

	return result;
}


void freeTree(Node* root) {
	if (root == NULL) return;

	freeTree(root -> one);
	freeTree(root -> zero);

	delete root;
}


/* Method creates (by map) char to bits converter by encoding tree */
void getCharConverter(Map <ext_char, string> &charToBits, Node* encodingTree, string current = "") {
	if (encodingTree -> character != NOT_A_CHAR) { // Base Case : it is a leave
		charToBits[encodingTree -> character] = current; 
		return;
	}
	getCharConverter(charToBits, encodingTree -> zero, current + "0");
	getCharConverter(charToBits, encodingTree -> one, current + "1");
}

/* Method writes each bit of bits in outfile */
void writeBits(string bits, obstream& outfile) {
	for (int i = 0; i < bits.length(); i++) {
		if (bits[i] == '0') outfile.writeBit(0);
		else outfile.writeBit(1);
	}
}

/* encode from infile to out file with encoding tree */
void encodeFile(istream& infile, Node* encodingTree, obstream& outfile) {
	/* key is character and value is string of appropriate bits */
	Map <ext_char, string> charToBits;
	getCharConverter(charToBits,encodingTree);

	char character;
	while (infile.get(character)) {
		if (!charToBits.containsKey(character)) error("Symbol can not found! ");
		writeBits(charToBits[character], outfile);
	}
	writeBits(charToBits[PSEUDO_EOF],outfile); 

}

/* Method copies info from first map to second map by reversed (key as value and value as key) */
void copyReversed(Map <ext_char, string> &charToBits, Map <string, ext_char> &bitsToChar) {
	foreach (ext_char character in charToBits) {
		string bit = charToBits[character];
		bitsToChar[bit] = character;
	}
}	

/* Method creates (by map) bits to char converter by encoding tree */
void getBitConverter(Map <string, ext_char> &bitsToChar, Node* encodingTree) {
	Map <ext_char, string> charToBits;
	getCharConverter(charToBits,encodingTree);
	copyReversed(charToBits, bitsToChar);
}

/* decode from infile to file with encoding tree */
void decodeFile(ibstream& infile, Node* encodingTree, ostream& file) {
	/* key is string of bits and value is appropriate charachter */
	Map <string, ext_char> bitsToChar;
	getBitConverter(bitsToChar, encodingTree);
	
	string bits = "";                   // initialize
	ext_char character = NOT_A_CHAR;   // bits and ext_char 
	while (character != PSEUDO_EOF) { // while it is not the end
		bits = bits + integerToString(infile.readBit());  // read next bit
		if (!bitsToChar.containsKey(bits)) continue;     // if no info about current bits read next bit
		if(character != NOT_A_CHAR) file.put((char)character);  // if it is not first visit HERE, write charachter
		character = bitsToChar[bits];                          // save next character
		bits = "";                                            // clear bits 
	}
}

/* Function: writeFileHeader
 * Usage: writeFileHeader(output, frequencies);
 * --------------------------------------------------------
 * Writes a table to the front of the specified output file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to decompress input files once they've been
 * compressed.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * readFileHeader function defined below this one so that it
 * can properly read the data back.
 */
void writeFileHeader(obstream& outfile, Map<ext_char, int>& frequencies) {
	/* The format we will use is the following:
	 *
	 * First number: Total number of characters whose frequency is being
	 *               encoded.
	 * An appropriate number of pairs of the form [char][frequency][space],
	 * encoding the number of occurrences.
	 *
	 * No information about PSEUDO_EOF is written, since the frequency is
	 * always 1.
	 */
	 
	/* Verify that we have PSEUDO_EOF somewhere in this mapping. */
	if (!frequencies.containsKey(PSEUDO_EOF)) {
		error("No PSEUDO_EOF defined.");
	}
	
	/* Write how many encodings we're going to have.  Note the space after
	 * this number to ensure that we can read it back correctly.
	 */
	outfile << frequencies.size() - 1 << ' ';
	
	/* Now, write the letter/frequency pairs. */
	foreach (ext_char ch in frequencies) {
		/* Skip PSEUDO_EOF if we see it. */
		if (ch == PSEUDO_EOF) continue;
		
		/* Write out the letter and its frequency. */
		outfile << char(ch) << frequencies[ch] << ' ';
	}
}

/* Function: readFileHeader
 * Usage: Map<ext_char, int> freq = writeFileHeader(input);
 * --------------------------------------------------------
 * Reads a table to the front of the specified input file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to reconstruct the encoding tree for that file.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * writeFileHeader function defined before this one so that it
 * can properly write the data.
 */
Map<ext_char, int> readFileHeader(ibstream& infile) {
	/* This function inverts the mapping we wrote out in the
	 * writeFileHeader function before.  If you make any
	 * changes to that function, be sure to change this one
	 * too!
	 */
	Map<ext_char, int> result;
	
	/* Read how many values we're going to read in. */
	int numValues;
	infile >> numValues;
	
	/* Skip trailing whitespace. */
	infile.get();
	
	/* Read those values in. */
	for (int i = 0; i < numValues; i++) {
		/* Get the character we're going to read. */
		ext_char ch = infile.get();
		
		/* Get the frequency. */
		int frequency;
		infile >> frequency;
		
		/* Skip the space character. */
		infile.get();
		
		/* Add this to the encoding table. */
		result[ch] = frequency;
	}
	
	/* Add in 1 for PSEUDO_EOF. */
	result[PSEUDO_EOF] = 1;
	return result;
}


void compress(ibstream& infile, obstream& outfile) {
	Map<ext_char, int> frequency = getFrequencyTable(infile);
	Node* encodingTree = buildEncodingTree(frequency);
	writeFileHeader(outfile,frequency);
	infile.rewind();
	encodeFile(infile, encodingTree, outfile);
	freeTree(encodingTree);
}


void decompress(ibstream& infile, ostream& outfile) {
	Node* encodingTree = buildEncodingTree(readFileHeader(infile));
	decodeFile(infile, encodingTree, outfile);
	freeTree(encodingTree);
}

