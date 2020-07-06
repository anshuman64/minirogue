//
//  Dungeon.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Dungeon_hpp
#define Dungeon_hpp

#include <stdio.h>
#include <vector>
#include <queue>
#include <string>
#include "utilities.hpp"

using namespace std;


class Object;
class GameObject;
class Room;
class Wall;
class Actor;
class Player;
class Monster;
class Weapon;

class Dungeon {
public:
  // Constructor
  Dungeon();
  ~Dungeon();
  
  // Accessors
  int     getLevel();
  Player* getPlayer();
  Object* getObject(int rowPos, int colPos);
  
  // Accessors - Booleans
  bool isGameOver();
  bool isRoom(int rowPos, int colPos);
  bool isWall(int rowPos, int colPos);
  bool isMonster(int rowPos, int colPos);
  bool isActor(int rowPos, int colPos);
  bool isPlayer(int rowPos, int colPos);
  bool isGameObject(int rowPos, int colPos);
    
  // Next & Update Level
  void nextLevel();
  void updateCurrentCell(Actor* actor, int curRow, int curCol);
  void updateNextCell(Actor* actor, int nextRow, int nextCol);
  
  // Display Level U Actions
  void displayLevel();
  void displayActions();
  void addAction(string action);
  
  // Monster Actions
  void moveMonsters();
  void destroyMonster(Monster* monster);
  
  // Win/Lose Game
  void endGame(string descrption);
  
private:
  bool          is_gameOver;
  int           m_level;
  Object*       m_maze[NUM_ROWS][NUM_COLS];
  queue<string> m_actions;
  
  Player*             m_player;
  Room*               m_rooms[NUM_ROWS][NUM_COLS];
  vector<Wall*>       m_walls;
  vector<Monster*>    m_monsters;
  vector<GameObject*> m_gameObjects;
  
  // Helpers - Create Level
  void createRooms();
  void createWalls();
  void createPlayer();
  void createMonsters();
  void createGameObjects();
  
  // Helpers - Reset Level
  void resetLevel();
  void resetCell(Object* object);
  
  // Helpers - Other
  void    setObjectPosition(Object* object);
  Weapon* generateRandomWeapon();
};


#endif /* Dungeon_hpp */
