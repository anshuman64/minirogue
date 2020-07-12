//
//  Object.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <iostream>
#include "Dungeon.hpp"

using namespace std;


// Parent class for all GameObjects, Monsters, and Player. Contains position, name, and symbol.
class Object {
public:
  // Constructor
  Object(Dungeon* dungeon, char symbol, string name);
  Object(Dungeon* dungeon, char symbol, string name, int rowPos, int colPos);
  virtual ~Object() {};
  
  // Accessors
  Dungeon* getDungeon();
  int      getRowPosition();
  int      getColPosition();
  char     getSymbol();
  string   getName();
  
  // Accessors - Booleans
  bool isActor();
  bool isPlayer();
  bool isMonster();
  bool isGameObject();
  bool isSpace();
  bool isWall();
  
  // Mutators
  void setPosition(int posRow, int posCol);
  
private:
  Dungeon* m_dungeon;
  string   m_name;
  int      m_pos_row;
  int      m_pos_col;
  char     m_symbol;
};

// Wall object class
class Wall : public Object {
public:
  Wall(Dungeon* dungeon, int rowPos, int colPos) : Object(dungeon, '#', "Wall", rowPos, colPos) {}
};


// Space (walkable area) object class
class Space : public Object {
public:
  Space(Dungeon* dungeon, int rowPos, int colPos) : Object(dungeon, ' ', "Space", rowPos, colPos) {}
};

#endif /* Object_hpp */
