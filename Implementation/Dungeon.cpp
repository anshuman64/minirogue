//
//  Dungeon.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include <iostream>
#include <set>

#include "Dungeon.hpp"
#include "Player.hpp"
#include "Object.hpp"
#include "Bogeyman.hpp"
#include "Snakewoman.hpp"
#include "Dragon.hpp"
#include "Goblin.hpp"
#include "Weapon.hpp"
#include "Scroll.hpp"
#include "utilities.hpp"

using namespace std;


// ******************************
// * Constructor
// ******************************

Dungeon::Dungeon() : m_level(0), is_gameOver(false), m_player(nullptr) {
  // WARNING: Has to be called in this order!
  createWalls();
  createSpaces();
  createMonsters();
  createGameObjects();
  createPlayer();
}

Dungeon::~Dungeon() {
  resetLevel();
  
  delete m_player;
}


// ******************************
// * Accessors
// ******************************

int Dungeon::getLevel() {
  return m_level;
}

Player* Dungeon::getPlayer() {
  return m_player;
}

Object* Dungeon::getObject(int rowPos, int colPos) {
  return m_maze[rowPos][colPos];
}


// ******************************
// * Accessors - Booleans
// ******************************

bool Dungeon::isGameOver() {
  return is_gameOver;
}

bool Dungeon::isSpace(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isSpace();
}

bool Dungeon::isWall(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isWall();
}

bool Dungeon::isMonster(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isMonster();
}

bool Dungeon::isActor(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isActor();
}

bool Dungeon::isPlayer(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isPlayer();
}

bool Dungeon::isGameObject(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isGameObject();
}

bool Dungeon::isMonstersRemaining() {
  return m_monsters.size() > 0;
}


// ******************************
// * Helpers - Create Level
// ******************************

void Dungeon::createWalls() {
  // Create a wall for every space in the level
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      Wall* newWall = new Wall(this, i, j);
      m_walls.push_back(newWall);
      m_maze[i][j] = newWall;
    }
  }
}

void Dungeon::createSpaces() {
  // Initialize spaces as nullptr
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      m_spaces[i][j] = nullptr;
    }
  }
  
  vector<vector<int>> rooms; // vector of rooms in the format: {startRow, startCol, endRow-1, endCol-1}
  
  // Generate non-overlapping rooms
  createRooms(rooms);
  
  int corridorTypes[rooms.size()][rooms.size()]; // 2D array to store direction of corridor between rooms
  
  // Initialize values for distances and corridorTypes
  for (int x = 0; x < rooms.size(); x++) {
    for (int y = 0; y < rooms.size(); y++) {
      corridorTypes[x][y] = -1;
    }
  }
  
  // Loop through every combination of rooms in both directions
  for (int x = 0; x < rooms.size(); x++) {
    for (int y = 0; y < rooms.size(); y++) {
      // If room is the same or corridor has already been made, continue
      if (x == y or corridorTypes[x][y] == -2) {
        continue;
      }
      
      int corridorType = -1;
      
      // Determine if corridor is possible from room x to y
      isCorridorType(corridorType, rooms, x, y, 0, 0, 1, 7, 2);
      isCorridorType(corridorType, rooms, x, y, 2, 0, 1, 6, 3);
      isCorridorType(corridorType, rooms, x, y, 1, 1, 0, 0, 5);
      isCorridorType(corridorType, rooms, x, y, 3, 1, 0, 1, 4);
      
      // If corridor is possible...
      if (corridorType > 0) {
        // Avoid creating corridor to the same room
        corridorTypes[y][x] = -2;
        
        // Create corridor between x and y
        switch (corridorType) {
          case 0:
            // Create corridor from top-left corner going up
            createCorridors(rooms[x][0], rooms[x][1], false, -1);
            break;
          case 1:
            // Create corridor from top-right corner going up
            createCorridors(rooms[x][0], rooms[x][3], false, -1);
            break;
          case 2:
            // Create corridor from top-right corner going right
            createCorridors(rooms[x][0], rooms[x][3], true, 1);
            break;
          case 3:
            // Create corridor from bottom-right corner going right
            createCorridors(rooms[x][2], rooms[x][3], true, 1);
            break;
          case 4:
            // Create corridor from bottom-right corner going down
            createCorridors(rooms[x][2], rooms[x][3], false, 1);
            break;
          case 5:
            // Create corridor from bottom-left corner going down
            createCorridors(rooms[x][2], rooms[x][1], false, 1);
            break;
          case 6:
            // Create corridor from bottom-left corner going left
            createCorridors(rooms[x][2], rooms[x][1], true, -1);
            break;
          case 7:
            // Create corridor from top-left corner going left
            createCorridors(rooms[x][0], rooms[x][1], true, -1);
            break;
        }
      }
    }
  }
}

// Creates non-overlapping rectangular rooms by brute-forcing generation of rectangles and seeing if they are valid
void Dungeon::createRooms(vector<vector<int>> &rooms) {
  int totalArea = 0;
  int tries = 0;
  
  while(totalArea < DESIRED_AREA and tries < 10000) {
    tries++; // Increase # of tries
    int startRow = randInt(NUM_ROWS-2-MIN_ROW_DIM)+1;
    int startCol = randInt(NUM_COLS-2-MIN_COL_DIM)+1;
    int endRow = startRow+randInt(6)+MIN_ROW_DIM;
    int endCol = startCol+randInt(10)+MIN_COL_DIM;
    
    // If room is on edge of level, invalidate room
    if (endRow > NUM_ROWS-1 or endCol > NUM_COLS-1) {
      continue;
    }
    
    // If room overlaps with existing spaces, invalidate room
    for (int i = 0; i < NUM_ROWS; i++) {
      for (int j = 0; j < NUM_COLS; j++) {
        if(m_spaces[i][j] != nullptr and i >= startRow-1 and i <= endRow+1 and j >= startCol-1 and j <= endCol+1) {
          goto cont; // "continue;" would break out of for loop, not while loop
        }
      }
    }

    // Else, create room
    rooms.push_back({startRow, startCol, endRow-1, endCol-1});
    for (int i = startRow; i < endRow; i++) {
      for (int j = startCol; j < endCol; j++) {
        Space* newSpace = new Space(this, i, j);
        m_spaces[i][j] = newSpace;
        m_maze[i][j] = newSpace;
      }
    }
    
    // Add room area to total area
    totalArea += (endRow-startRow) * (endCol-startCol);
    cont:;
  }
}

// Determine if corridor is possible from room x to y by using their start & end rows/cols
// a, b, c denote which row/cols to test for overlap
// m and n denote corresponding corridorType (0 to 7 clockwise from top-left corner of room)
// Store the distance and corridorType into variables
void Dungeon::isCorridorType(int &corridorType, vector<vector<int>> rooms, int x, int y, int a, int b, int c, int m, int n) {
  if (rooms[x][a] >= rooms[y][b] and rooms[x][a] <= rooms[y][b+2]) {
    if (rooms[x][c] > rooms[y][c]) {
      corridorType = m;
    } else {
      corridorType = n;
    }
  }
}

//
void Dungeon::createCorridors(int curRow, int curCol, bool changeCol, int delta) {
  // Start by offsetting corner of room by one
  if (changeCol) {
    curCol += delta;
  } else {
    curRow += delta;
  }
  
  // While cell isWall and is not on the edge of the level
  while (isWall(curRow, curCol) and curRow > 0 and curRow < NUM_ROWS and curCol > 0 and curCol < NUM_COLS) {
    // Create a new space at current cell
    Space* newSpace = new Space(this, curRow, curCol);
    m_spaces[curRow][curCol] = newSpace;
    m_maze[curRow][curCol] = newSpace;
    
    // Update cell in correct direction
    if (changeCol) {
      curCol += delta;
    } else {
      curRow += delta;
    }
  }
}

void Dungeon::createPlayer() {
  m_player = new Player(this);
  setObjectPosition(m_player);
}

void Dungeon::createMonsters() {
  int numMonsters = randInt(m_level * 5 - 1) + 2;
  
  for (int i = 0; i <= numMonsters; i++) {
    Monster* addMonster = generateRandomMonster();
    m_monsters.push_back(addMonster);
    setObjectPosition(addMonster);
  }
}

void Dungeon::createGameObjects() {
  int numGameObjects = randInt(2) + m_level;
  
  for (int i = 0; i < numGameObjects; i++) {
    GameObject* addGameObject = generateRandomGameObject();
    m_gameObjects.push_back(addGameObject);
    setObjectPosition(addGameObject);
  }
  
  // Creates stairs to next level
  if (m_level != MAX_LEVEL) {
    GameObject* stairs = new GameObject(this, '>', "Stairs");
    m_gameObjects.push_back(stairs);
    setObjectPosition(stairs);
  }
  
  // Creates golden idol
  if (m_level == MAX_LEVEL) {
    GameObject* goldenIdol = new GameObject(this, '&', "Golden Idol");
    m_gameObjects.push_back(goldenIdol);
    setObjectPosition(goldenIdol);
  }
}

void Dungeon::setObjectPosition(Object* object) {
  int posRow = -1;
  int posCol = -1;
  generateRandomPosition(object, posRow, posCol);
  object->setPosition(posRow, posCol);
  
  if (object->isGameObject() and isActor(posRow, posCol)) {
    Actor* actor = (Actor*)getObject(posRow, posCol);
    actor->setOverGameObject((GameObject*)object);
  } else {
    m_maze[posRow][posCol] = object;
  }
}

void Dungeon::generateRandomPosition(Object* object, int &posRow, int &posCol) {
  while(true) {
    posRow = randInt(NUM_ROWS);
    posCol = randInt(NUM_COLS);
    
    if (!isWall(posRow, posCol) and (
      (object->isActor() and !isActor(posRow, posCol)) or
      (object->isGameObject() and !isGameObject(posRow, posCol)) )) {
      return;
    }
  }
}

Monster* Dungeon::generateRandomMonster() {
  int numChoices = m_level >=3 ? 4 : 3;
  switch (randInt(numChoices)) {
    case 0:
      return new Bogeyman(this);
      break;
    case 1:
      return new Snakewoman(this);
      break;
    case 2:
      return new Goblin(this);
      break;
    case 3:
      return new Dragon(this);
      break;
  }
  
  return new Bogeyman(this);
}

GameObject* Dungeon::generateRandomGameObject() {
  switch (randInt(7)) {
    case 0:
      return new Mace(this);
      break;
    case 1:
      return new ShortSword(this);
      break;
    case 2:
      return new LongSword(this);
      break;
    case 3:
      return new HealthScroll(this);
      break;
    case 4:
      return new ArmorScroll(this);
      break;
    case 5:
      return new StrengthScroll(this);
      break;
    case 6:
      return new DexterityScroll(this);
      break;
  }
  
  return new ShortSword(this);
}


// ******************************
// * Reset & Next Level
// ******************************

void Dungeon::nextLevel() {
  resetLevel();
  m_level++;
  
  createWalls();
  createSpaces();
  createMonsters();
  createGameObjects();
  setObjectPosition(m_player);
}

void Dungeon::resetLevel() {
  while (!m_monsters.empty()) {
    m_monsters.pop_back();
  }
  
  while (!m_gameObjects.empty()) {
    m_gameObjects.pop_back();
  }
  
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      delete m_spaces[i][j];
      m_spaces[i][j] = nullptr;
    }
  }
  
  for (int i = 0; i < m_walls.size(); i++) {
    delete m_walls[i];
    m_walls[i] = nullptr;
  }
}


// ******************************
// * Update Cells (from Actor movement)
// ******************************

void Dungeon::updateCurrentCell(Actor* actor, int curRow, int curCol) {
  if (actor->isOverGameObject()) {
    m_maze[curRow][curCol] = actor->getOverGameObject();
  } else {
    m_maze[curRow][curCol] = m_spaces[curRow][curCol];
  }
}

void Dungeon::updateNextCell(Actor* actor, int nextRow, int nextCol) {
  m_maze[nextRow][nextCol] = actor;
}


// ******************************
// * Display Level & Actions
// ******************************

void Dungeon::displayLevel() {
  clearScreen();
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      cout << m_maze[i][j]->getSymbol();
    }
    cout << endl;
  }
  cout << endl;
}

void Dungeon::displayActions() {
  while(!m_actions.empty()) {
    cout << m_actions.front() << endl;
    m_actions.pop();
  }
  
  cout << endl;
}

void Dungeon::addAction(string action) {
  if (!is_gameOver) {
    m_actions.push(action);
  }
}


// ******************************
// * Monster Actions
// ******************************

void Dungeon::moveMonsters(){
  for (int i = 0; i < m_monsters.size(); i++) {
    m_monsters[i]->calculateMove();
    
    if (is_gameOver) {
      return;
    }
  }
}

void Dungeon::destroyMonster(Monster* monster) {
  addAction(monster->getName() + " is defeated!");
  
  for (int i = 0; i < m_monsters.size(); i++) {
    if (m_monsters[i] == monster) {
      int rowPos = monster->getRowPosition();
      int colPos = monster->getColPosition();
      
      // Check if monster drops game object
      GameObject* droppedGameObject = monster->dropGameObject();
      if (droppedGameObject != nullptr) {
        m_gameObjects.push_back(droppedGameObject);
        m_maze[rowPos][colPos] = droppedGameObject;
        droppedGameObject->setPosition(rowPos, colPos);
      } else {
        updateCurrentCell(monster, rowPos, colPos);
      }
      
      m_monsters.erase(m_monsters.begin() + i);
      delete monster;
    }
  }
}


// ******************************
// * Win/Lose Game
// ******************************

void Dungeon::endGame(string description) {
  addAction(description);
  is_gameOver = true;
}
