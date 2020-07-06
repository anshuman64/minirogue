//
//  Object.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include "Object.hpp"

// ******************************
// * Constructor
// ******************************

Object::Object(Dungeon* dungeon, char symbol, string name) : m_dungeon(dungeon), m_symbol(symbol), m_name(name), m_pos_row(-1), m_pos_col(-1) {}

Object::Object(Dungeon* dungeon, char symbol, string name, int rowPos, int colPos) : m_dungeon(dungeon), m_symbol(symbol), m_name(name), m_pos_row(rowPos), m_pos_col(colPos) {}


// ******************************
// * Accessors
// ******************************

Dungeon* Object::getDungeon() {
  return m_dungeon;
}

int Object::getRowPosition() {
  return m_pos_row;
}

int Object::getColPosition() {
  return m_pos_col;
}

char Object::getSymbol() {
  return m_symbol;
}

string Object::getName() {
  return m_name;
}


// ******************************
// * Accessors - Booleans
// ******************************

bool Object::isActor() {
  return (isMonster() or isPlayer());
}

bool Object::isPlayer() {
  return (m_symbol == '@');
}

bool Object::isMonster() {
  return (m_symbol == 'B' or m_symbol == 'D' or m_symbol == 'G' or m_symbol == 'S');
}

bool Object::isGameObject() {
  return (m_symbol == '?' or m_symbol == ')' or m_symbol == '&' or m_symbol == '>');
}

bool Object::isRoom() {
  return (m_symbol == ' ');
}

bool Object::isWall() {
  return (m_symbol == '#');
}


// ******************************
// * Mutators
// ******************************

void Object::setPosition(int posRow, int posCol) {
  m_pos_row = posRow;
  m_pos_col = posCol;
}
