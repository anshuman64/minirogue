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

Dungeon::Dungeon() : m_level(0), is_gameOver(false) {
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


// ******************************
// * Helpers - Create Level
// ******************************

void Dungeon::createWalls() {
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
  
  vector<vector<int>> rooms;
  
  // Generate non-overlapping rooms
  createRooms(rooms, 350);
  
  int distances[rooms.size()][rooms.size()];
  int corridorTypes[rooms.size()][rooms.size()];
  
  for (int i = 0; i < rooms.size()-1; i++) {
    for (int j = i+1; j < rooms.size(); j++) {
      // Determine if corridor is possible from room i to room j
      int distance = NUM_COLS;
      int corridorType = -1;
      isCorridorType(distance, corridorType, rooms, i, j, 0, 0, 1, 7, 2);
      isCorridorType(distance, corridorType, rooms, i, j, 2, 0, 1, 6, 3);
      isCorridorType(distance, corridorType, rooms, i, j, 1, 1, 0, 0, 5);
      isCorridorType(distance, corridorType, rooms, i, j, 3, 1, 0, 1, 4);
      distances[i][j] = distance;
      corridorTypes[i][j] = corridorType;
      
      // Determine if corridor is possible from room j to room i
      distance = NUM_COLS;
      corridorType = -1;
      isCorridorType(distance, corridorType, rooms, j, i, 0, 0, 1, 7, 2);
      isCorridorType(distance, corridorType, rooms, j, i, 2, 0, 1, 6, 3);
      isCorridorType(distance, corridorType, rooms, j, i, 1, 1, 0, 0, 5);
      isCorridorType(distance, corridorType, rooms, j, i, 3, 1, 0, 1, 4);
      distances[j][i] = distance;
      corridorTypes[j][i] = corridorType;
    }
  }
  
  for (int x = 0; x < rooms.size(); x++) {
    int corridorType = -1;
    
    for (int y = 0; y < rooms.size(); y++) {
      if (x == y) {
        continue;
      }
      
      if (corridorTypes[x][y] > 0) {
        corridorType = corridorTypes[x][y];
        
        // Avoid creating corridor to the same room
        distances[y][x] = NUM_COLS;
        corridorTypes[y][x] = -1;
      }
      
      switch (corridorType) {
        case 0:
          createCorridors(rooms, rooms[x][0], rooms[x][1], false, -1);
          break;
        case 1:
          createCorridors(rooms, rooms[x][0], rooms[x][3], false, -1);
          break;
        case 2:
          createCorridors(rooms, rooms[x][0], rooms[x][3], true, 1);
          break;
        case 3:
          createCorridors(rooms, rooms[x][2], rooms[x][3], true, 1);
          break;
        case 4:
          createCorridors(rooms, rooms[x][2], rooms[x][3], false, 1);
          break;
        case 5:
          createCorridors(rooms, rooms[x][2], rooms[x][1], false, 1);
          break;
        case 6:
          createCorridors(rooms, rooms[x][2], rooms[x][1], true, -1);
          break;
        case 7:
          createCorridors(rooms, rooms[x][0], rooms[x][1], true, -1);
          break;
      }
    }
  }
}

void Dungeon::createRooms(vector<vector<int>> &rooms, int desiredArea) {
  int totalArea = 0;
  int MIN_ROW_DIM = 5;
  int MIN_COL_DIM = 7;
  int tries = 0;
  
  while(totalArea < desiredArea and tries < 10000) {
    tries++;
    int startRow = randInt(NUM_ROWS-2-MIN_ROW_DIM)+1;
    int startCol = randInt(NUM_COLS-2-MIN_COL_DIM)+1;
    int endRow = startRow+randInt(6)+MIN_ROW_DIM;
    int endCol = startCol+randInt(10)+MIN_COL_DIM;
    
    if (endRow > NUM_ROWS-1 or endCol > NUM_COLS-1) {
      continue;
    }
    
    for (int i = 0; i < NUM_ROWS; i++) {
      for (int j = 0; j < NUM_COLS; j++) {
        if(m_spaces[i][j] != nullptr and i >= startRow-1 and i <= endRow+1 and j >= startCol-1 and j <= endCol+1) {
          goto cont;
        }
      }
    }

    rooms.push_back({startRow, startCol, endRow-1, endCol-1});
    for (int i = startRow; i < endRow; i++) {
      for (int j = startCol; j < endCol; j++) {
        Space* newSpace = new Space(this, i, j);
        m_spaces[i][j] = newSpace;
        m_maze[i][j] = newSpace;
      }
    }
    
    totalArea += (endRow-startRow) * (endCol-startCol);
    cont:;
  }
}
              
void Dungeon::isCorridorType(int &distance, int &corridorType, vector<vector<int>> rooms, int x, int y, int a, int b, int c, int m, int n) {
  if (rooms[x][a] >= rooms[y][b] and rooms[x][a] <= rooms[y][b+2]) {
    if (rooms[x][c] > rooms[y][c]) {
      distance = rooms[x][c] - rooms[y][c+2];
      corridorType = m;
    } else {
      distance = rooms[y][c] - rooms[x][c+2];
      corridorType = n;
    }
  }
}

void Dungeon::createCorridors(vector<vector<int>> &rooms, int curRow, int curCol, bool changeCol, int delta) {
  // Start by offsetting corner of room by one
  if (changeCol) {
    curCol += delta;
  } else {
    curRow += delta;
  }
  
  while (isWall(curRow, curCol) and curRow > 0 and curRow < NUM_ROWS and curCol > 0 and curCol < NUM_COLS) {
    Space* newSpace = new Space(this, curRow, curCol);
    m_spaces[curRow][curCol] = newSpace;
    m_maze[curRow][curCol] = newSpace;
    
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
  int numGameObjects = randInt(2) + 2;
  
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
  m_maze[posRow][posCol] = object;
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
  switch (randInt(numChoices-1)) {
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
  m_actions.push(action);
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
