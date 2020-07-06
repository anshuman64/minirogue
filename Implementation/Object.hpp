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


class Object {
public:
  // Constructor
  Object(Dungeon* dungeon, char symbol, string name);
  Object(Dungeon* dungeon, char symbol, string name, int rowPos, int colPos);
  virtual ~Object() {};
  
  // Accessors
  Dungeon* getDungeon();
  int  getRowPosition();
  int  getColPosition();
  char getSymbol();
  string getName();
  
  // Accessors - Booleans
  bool isActor();
  bool isPlayer();
  bool isMonster();
  bool isGameObject();
  bool isRoom();
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

#endif /* Object_hpp */
