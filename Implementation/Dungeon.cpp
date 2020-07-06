//
//  Dungeon.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include <iostream>

#include "Dungeon.hpp"
#include "Player.hpp"
#include "Room.hpp"
#include "Wall.hpp"
#include "Bogeyman.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"

using namespace std;


// ******************************
// * Constructor
// ******************************

Dungeon::Dungeon() : m_level(0), is_gameOver(false) {
  createLevel();
}

Dungeon::~Dungeon() {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      delete m_rooms[i][j];
    }
  }
  
  for (int i = 0; i < m_walls.size(); i++) {
    delete m_walls[i];
  }
  
  for (int i = 0; i < m_monsters.size(); i++) {
    delete m_monsters[i];
  }
  
  for (int i = 0; i < m_gameObjects.size(); i++) {
    delete m_gameObjects[i];
  }
  
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

bool Dungeon::isRoom(int rowPos, int colPos) {
  return m_maze[rowPos][colPos]->isRoom();
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

// WARNING: Has to be called in this order!
void Dungeon::createLevel() {
  createRooms();
  createWalls();
  createPlayer();
  createMonsters();
  createGameObjects();
}

void Dungeon::createRooms() {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      Room* newRoom = new Room(this, i, j);
      m_rooms[i][j] = newRoom;
      m_maze[i][j] = newRoom;
    }
  }
}

void Dungeon::createWalls() {
  for (int i = 0; i < NUM_ROWS; i++) {
    for (int j = 0; j < NUM_COLS; j++) {
      if (i == 0 or i == NUM_ROWS-1 or j == 0 or j == NUM_COLS-1) {
        Wall* newWall = new Wall(this, i, j);
        m_walls.push_back(newWall);
        m_maze[i][j] = newWall;
      }
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
    m_monsters.push_back(new Bogeyman(this));
    setObjectPosition(m_monsters[i]);
  }
}

void Dungeon::createGameObjects() {
  int numGameObjects = randInt(2) + 2;
  
  for (int i = 0; i < numGameObjects; i++) {
    m_gameObjects.push_back(new Weapon(this, "Long Sword", 3, 2, "slashes"));
    setObjectPosition(m_gameObjects[i]);
  }
  
  // Creates golden idol
  GameObject* goldenIdol = new GameObject(this, '&', "Golden Idol", "Idol with magic powers. Pick up to win the game!");
  m_gameObjects.push_back(goldenIdol);
  setObjectPosition(goldenIdol);
}

void Dungeon::setObjectPosition(Object* object) {
  int posRow, posCol;
  
  while(true) {
    posRow = randInt(NUM_ROWS);
    posCol = randInt(NUM_COLS);
    
    if (!isWall(posRow, posCol) and (
      (object->isActor() and !isActor(posRow, posCol)) or
      (object->isGameObject() and !isGameObject(posRow, posCol)) )) {
      object->setPosition(posRow, posCol);
      m_maze[posRow][posCol] = object;
      return;
    }
  }
}


// ******************************
// * Update and Display Level
// ******************************

void Dungeon::updateCurrentCell(Actor* actor, int curRow, int curCol) {
  if (actor->isOverGameObject()) {
    m_maze[curRow][curCol] = actor->getOverGameObject();
  } else {
    m_maze[curRow][curCol] = m_rooms[curRow][curCol];
  }
}

void Dungeon::updateNextCell(Actor* actor, int nextRow, int nextCol) {
  m_maze[nextRow][nextCol] = actor;
}

void Dungeon::moveMonsters(){
  for (int i = 0; i < m_monsters.size(); i++) {
    m_monsters[i]->calculateMove();
    
    if (is_gameOver) {
      return;
    }
  }
}

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


// ******************************
// * Display Actions
// ******************************

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
// * Mutators - Destroy Objects
// ******************************

void Dungeon::destroyMonster(Monster* monster) {
  addAction(monster->getName() + " is defeated!");
  
  for (int i = 0; i < m_monsters.size(); i++) {
    if (m_monsters[i] == monster) {
      int rowPos = monster->getRowPosition();
      int colPos = monster->getColPosition();
      m_maze[rowPos][colPos] = m_rooms[rowPos][colPos];
      
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
