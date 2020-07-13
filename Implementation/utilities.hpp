// utilities.h

// Note:  YOU MUST NOT MAKE ANY CHANGE TO THIS FILE!

#ifndef utilities_hpp
#define utilities_hpp

#include <stdio.h>
#include <list>

using namespace std;

// Movement constants
const char ARROW_LEFT  = 'a';
const char ARROW_RIGHT = 'd';
const char ARROW_UP    = 'w';
const char ARROW_DOWN  = 's';

// Dungeon constants
const int NUM_ROWS = 18;
const int NUM_COLS = 70;
const int DESIRED_AREA = 300; // minimum area of free space rooms should cover
const int MIN_ROW_DIM = 5; // minimum height of rooms
const int MIN_COL_DIM = 7; // minimum width of rooms

// Player constants
const int MAX_STATS = 99;
const int MAX_LEVEL = 4;

// Monster constants
const int BOGEYMAN_SMELL_RANGE = 5;
const int SNAKEWOMAN_SMELL_RANGE = 3;
const int GOBLIN_SMELL_RANGE = 10;

// Functions
char getCharacter();
void clearScreen();
int randInt(int limit);             // random int from 0 to limit-1
bool trueWithProbability(double p); // return true with probability p


#endif /* utilities_hpp */
