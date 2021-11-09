#include <iostream>
#include "gboggle.h"
#include "grid.h"
#include "gwindow.h"
#include "lexicon.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"
#include <vector>
#include "set.h"
#include "strlib.h"
using namespace std;

/* Constants */
const int N_ROWS_COLS_OF_BOARD = 4;
const int LENGTH_OF_CUBE = 6;
const int MIN_LENGTH_OF_WORD = 4;
const int BOGGLE_WINDOW_WIDTH = 650;
const int BOGGLE_WINDOW_HEIGHT = 350;

const string STANDARD_CUBES[16]  = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
 
const string BIG_BOGGLE_CUBES[25]  = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

struct coordinates {
	int i;
	int j;
	coordinates(int arg1, int arg2) {
		i = arg1;
		j = arg2;
	}
};

/* Function prototypes */

void welcome();
void giveInstructions();
void gamePlay(Lexicon &lexicon);
void getBoard(Grid<char> &board);
bool userWantsTo(string question);
void fillCustomCubes(Vector<string> &customCubes);
string getLegalCube();
void fillDefaultCubes(Vector<string> &defaultCubes);
/* Method fills board by cubes */
void fillBoardBy(Grid<char> &board, Vector<string> &cubes);
/* Method fills board at position by random char of cube */
void saveRandomSide(Grid<char> &board, int position, string cube);
void displayBoard(Grid<char> &board);
void playerTurn(Grid<char> &board, Set<string> &playerGuessed, Lexicon &lexicon);
bool isOnBoard(string word, Grid<char> &board, vector<coordinates> &usedCoordinates,coordinates start = coordinates(0,0),coordinates end = coordinates(N_ROWS_COLS_OF_BOARD - 1, N_ROWS_COLS_OF_BOARD - 1));
bool containsCoordinate(coordinates cord, vector<coordinates> &vec);
void displayHighlitedWord (vector<coordinates> &usedCoordinates);
void computerTurn(Grid<char> &board, Set<string> &guessed, Lexicon &lexicon);
void findAllWord(Grid<char> &board, Set<string> &guessed, Lexicon &lexicon, vector<coordinates> &usedCoordinates, string currentWord = "", coordinates start = coordinates(0,0) , coordinates end = coordinates(N_ROWS_COLS_OF_BOARD - 1, N_ROWS_COLS_OF_BOARD - 1));




/* Main program */

int main() {
    GWindow gw(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT);
    initGBoggle(gw);
    welcome();
    giveInstructions();

	Lexicon lexicon("EnglishWords.dat");

	gamePlay(lexicon);
	

    return 0;
}

void gamePlay(Lexicon &lexicon) {
	while (true) {
		Set<string> playerGuessed;
		Grid<char> board(N_ROWS_COLS_OF_BOARD, N_ROWS_COLS_OF_BOARD);
		getBoard(board);
		
		displayBoard(board);

		playerTurn(board, playerGuessed, lexicon);
		computerTurn(board, playerGuessed, lexicon);
		if (!userWantsTo("Would you like to play again? "))
			break;
	}
}

/*
 * Function: welcome
 * Usage: welcome();
 * -----------------
 * Print out a cheery welcome message.
 */

void welcome() {
    cout << "Welcome!  You're about to play an intense game ";
    cout << "of mind-numbing Boggle.  The good news is that ";
    cout << "you might improve your vocabulary a bit.  The ";
    cout << "bad news is that you're probably going to lose ";
    cout << "miserably to this little dictionary-toting hunk ";
    cout << "of silicon.  If only YOU had a gig of RAM..." << endl << endl;
}

/*
 * Function: giveInstructions
 * Usage: giveInstructions();
 * --------------------------
 * Print out the instructions for the user.
 */

void giveInstructions() {
    cout << endl;
    cout << "The boggle board is a grid onto which I ";
    cout << "I will randomly distribute cubes. These ";
    cout << "6-sided cubes have letters rather than ";
    cout << "numbers on the faces, creating a grid of ";
    cout << "letters on which you try to form words. ";
    cout << "You go first, entering all the words you can ";
    cout << "find that are formed by tracing adjoining ";
    cout << "letters. Two letters adjoin if they are next ";
    cout << "to each other horizontally, vertically, or ";
    cout << "diagonally. A letter can only be used once ";
    cout << "in each word. Words must be at least four ";
    cout << "letters long and can be counted only once. ";
    cout << "You score points based on word length: a ";
    cout << "4-letter word is worth 1 point, 5-letters ";
    cout << "earn 2 points, and so on. After your puny ";
    cout << "brain is exhausted, I, the supercomputer, ";
    cout << "will find all the remaining words and double ";
    cout << "or triple your paltry score." << endl << endl;
    cout << "Hit return when you're ready...";
    getLine();
}

void getBoard(Grid<char> &board) {
	Vector<string> cubes(N_ROWS_COLS_OF_BOARD * N_ROWS_COLS_OF_BOARD, "");
	if (userWantsTo("Do you want to force the board configuration? ")) {
		fillCustomCubes(cubes);
	} else {
		fillDefaultCubes(cubes);
	}
	fillBoardBy(board,cubes);
}

/* Method return true if user wants to enter custom cubes */
bool userWantsTo(string question) {
	string answer = getLine(question);

	if (answer == "yes" || answer == "YES") return true;

	if (answer == "no" || answer == "NO") return false;

	return userWantsTo(question);

} 

void fillCustomCubes(Vector<string> &customCubes) {
	for (int i = 0; i < customCubes.size(); i++) {
		string cube = getLegalCube();
		customCubes[i] = cube;
	}
}

/* Method returns certain length of upperCase string */
string getLegalCube() {
	while (true) {
		string cube = getLine("Enter Cube: ");
		if (cube.length() >= LENGTH_OF_CUBE)
			return toUpperCase(cube.substr(0,LENGTH_OF_CUBE)); 
	}
}

/* copying array to vec */
void fillDefaultCubes(Vector<string> &defaultCubes) {
	for (int i = 0; i < defaultCubes.size(); i++) {
		defaultCubes[i] = STANDARD_CUBES[i];
	}
}

/* Method fills grid by vec of cubes */
void fillBoardBy(Grid<char> &board, Vector<string> &cubes) {
	for (int i = 0; i < cubes.size(); i++) {
		int randomIndex = randomInteger(i,cubes.size() - 1);
		string randomCube = cubes[randomIndex];
		saveRandomSide(board, i, randomCube); 
		swap(cubes[i],cubes[randomIndex]);
	}

}

/* Method saves random side of cube at position of board */
void saveRandomSide(Grid<char> &board, int position, string cube) {
	int randomIndex = randomInteger(0,cube.length() - 1);
	char randomChar = cube[randomIndex];
	int i = position / N_ROWS_COLS_OF_BOARD;
	int j = position % N_ROWS_COLS_OF_BOARD;
	board[i][j] = randomChar;
}

/* display letters of board */
void displayLabels(Grid<char> &board) {
	for (int i = 0; i < N_ROWS_COLS_OF_BOARD; i++) {
		for (int j = 0; j < N_ROWS_COLS_OF_BOARD; j++) {
			char letter = board[i][j];
			labelCube(i,j,letter);
		}
	}
}

void displayBoard(Grid<char> &board){
	drawBoard(N_ROWS_COLS_OF_BOARD,N_ROWS_COLS_OF_BOARD);
	displayLabels(board);
}

/* Word is valid if it is in lexicon, meets min length and not used yet. last argument is true if you want to print reason of not valid*/
bool isValidWord(string word, Lexicon &lexicon, Set<string> &playerGuessed, bool print = true) {
	if (word.length() < MIN_LENGTH_OF_WORD) {
		if (print)
			cout << "That word doesn't meet the minimum word length." << endl;
		return false;	
	}
	if (playerGuessed.contains(toUpperCase(word))) {
		if (print)
			cout << "You've already guessed that!" << endl;
		return false;
	}
	if (!lexicon.contains(word)) {
		if (print)
			cout << "That's not a word!" << endl;
		return false;
	}

	return true;

}


void playerTurn(Grid<char> &board, Set<string> &playerGuessed, Lexicon &lexicon) {
	cout << " Ok, take all the time you want and find all the words you can! Signal that you're finished by entering an empty line. " << endl;
	
	while (true) {
		string word = getLine("Enter a word: ");
		if (word.length() == 0) break;

		if (!isValidWord(word, lexicon, playerGuessed))
			continue;

		vector<coordinates> usedCoordinates; // for not using same coordinates for current word 
		if (isOnBoard(toUpperCase(word), board, usedCoordinates)) {
			playerGuessed.add(toUpperCase(word));
			displayHighlitedWord(usedCoordinates);
			recordWordForPlayer(word,HUMAN);
		} else {
			cout << "You can't make that word!" << endl;
		}
	}	
	
}

/* Method gets new start and end coordinates. */
void changeCoordinates(coordinates &start, coordinates &end, Grid<char> &board) {
	if (board.inBounds(start.i - 1, start.j)) 
		start.i--;
	if (board.inBounds(start.i, start.j - 1)) 
		start.j--;
	if (board.inBounds(end.i + 1,end.j))
		end.i++;
	if (board.inBounds(end.i,end.j + 1))
		end.j++;
}

/* Method checks if vector contains coordinate */
bool containsCoordinate(coordinates cord, vector<coordinates> &vec) {
	for (int i = 0; i < vec.size(); i++) {
		coordinates currentCord = vec.at(i);
		if (cord.i == currentCord.i && cord.j == currentCord.j)
			return true;
	}
	return false;
}

/* Method returns true if there is the word from starging to ending on the board */
bool isOnBoard(string word, Grid<char> &board, vector<coordinates> &usedCoordinates, coordinates start, coordinates end) {
	if (word.length() == 0) // Base Case :  empty string 
		return true;
	
	for (int row = start.i; row <= end.i; row++) {
		for (int col = start.j; col <= end.j; col++) { // if current letter is first char of word & is not used yet
			if (board[row][col] == word[0] && !containsCoordinate(coordinates(row,col), usedCoordinates)) {
				usedCoordinates.push_back(coordinates(row,col)); 
				coordinates newStart(row,col);
				coordinates newEnd(row,col);
				changeCoordinates(newStart, newEnd, board); // get new start and new end
				if (isOnBoard(word.substr(1), board, usedCoordinates, newStart, newEnd)) { // continue searching to rest of
					return true;
				}
				usedCoordinates.pop_back(); // if it was not successful way undo
			}
		}
	}
	return false;
}

/* Method on/of every used coordinates */
void highlightWord (vector<coordinates> &usedCoordinates, bool flag) {
	for (int i = 0; i < usedCoordinates.size(); i++) {
		highlightCube(usedCoordinates.at(i).i,usedCoordinates.at(i).j,flag);
	}
}


void displayHighlitedWord (vector<coordinates> &usedCoordinates) {
	highlightWord(usedCoordinates,true);
	pause(50);
	highlightWord(usedCoordinates,false);
}

void computerTurn(Grid<char> &board, Set<string> &guessed, Lexicon &lexicon) {
	vector<coordinates> usedCoordinates;
	findAllWord(board, guessed, lexicon,usedCoordinates);
	
}

/* Method finds all word on board that guessed do not contains */
void findAllWord(Grid<char> &board, Set<string> &guessed, Lexicon &lexicon, vector<coordinates> &usedCoordinates, string currentWord, coordinates start, coordinates end) {
	if (!lexicon.containsPrefix(currentWord))
		return;

	// if it is valid word save it
	if(isValidWord(currentWord, lexicon, guessed,false)) {
		guessed.add(currentWord);
		recordWordForPlayer(currentWord,COMPUTER);
	}
	// continue searching
	for (int row = start.i; row <= end.i; row++) {
		for (int col = start.j; col <= end.j; col++) {
			if (!containsCoordinate(coordinates(row,col), usedCoordinates)) {
				usedCoordinates.push_back(coordinates(row,col));
				string newCurrent = currentWord + board[row][col]; // new word
				coordinates newStart(row,col);
				coordinates newEnd(row,col);
				changeCoordinates(newStart, newEnd, board); // possible ways to continue
				findAllWord(board, guessed, lexicon, usedCoordinates, newCurrent, newStart, newEnd);
				usedCoordinates.pop_back(); // undo
			}
		}
	}
}