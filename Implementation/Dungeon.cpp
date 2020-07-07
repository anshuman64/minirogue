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
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      delete m_spaces[i][j];
    }
  }
  
  for (int i = 0; i < m_walls.size(); i++) {
    delete m_walls[i];
  }
  
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
  int totalArea = 0;
  int startRow = 1;
  int startCol = 1;
  int endRow = 4;
  int endCol = 4;
  set<int> usedRows;
  set<int> usedCols;
  
  while(totalArea < 300) {
    startRow = randInt(NUM_ROWS-2)+1;
    startCol = randInt(NUM_COLS-2)+1;
    endRow = startRow+randInt(6)+4;
    endCol = startCol+randInt(10)+4;
    
    if (endRow > NUM_ROWS-1 or endCol > NUM_COLS-1) {
      continue;
    }
    
    
    
    for (int i = startRow; i < endRow; i++) {
      for (int j = startCol; j < endCol; j++) {
        Space* newSpace = new Space(this, i, j);
        m_spaces[i][j] = newSpace;
        m_maze[i][j] = newSpace;
      }
    }
    
    totalArea += (endRow-startRow) * (endCol-startCol);
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
  
  createMonsters();
  createGameObjects();
  setObjectPosition(m_player);
}

void Dungeon::resetLevel() {
  while (!m_monsters.empty()) {
    resetCell(m_monsters.back());
    m_monsters.pop_back();
  }
  
  while (!m_gameObjects.empty()) {
    resetCell(m_gameObjects.back());
    m_gameObjects.pop_back();
  }
  
  resetCell(m_player);
}

void Dungeon::resetCell(Object* object) {
  int objectRowPos = object->getRowPosition();
  int objectColPos = object->getColPosition();
  m_maze[objectRowPos][objectColPos] = m_spaces[objectRowPos][objectColPos];
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
