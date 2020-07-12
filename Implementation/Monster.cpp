//
//  Monster.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include "Monster.hpp"
#include "Player.hpp"


// ******************************
// * Constructor
// ******************************

Monster::Monster(Dungeon* dungeon, char symbol, string name, int hp, int armor, int strength, int dexterity) : Actor(dungeon, symbol, name, hp, armor, strength, dexterity) {}


// ******************************
// * Helpers
// ******************************

int Monster::rowsToPlayer() {
  int playerRow = getDungeon()->getPlayer()->getRowPosition();
  
  return playerRow - getRowPosition();
}

int Monster::colsToPlayer() {
  int playerCol = getDungeon()->getPlayer()->getColPosition();
  
  return playerCol - getColPosition();
}


// ******************************
// * Actions
// ******************************

int Monster::stepsToPlayer() {
  return abs(rowsToPlayer()) + abs(colsToPlayer());
}

// Dumb move method for Bogeymen and Snakewomen
void Monster::attemptMove() {
  int curRow = getRowPosition();
  int curCol = getColPosition();
  int nextRow = getRowPosition();
  int nextCol = getColPosition();
  
  // Determine direction to move
  if (rowsToPlayer() > 0) {
    nextRow++;
  } else if (rowsToPlayer() < 0) {
    nextRow--;
  }
  
  if (colsToPlayer() > 0) {
    nextCol++;
  } else if (colsToPlayer() < 0) {
    nextCol--;
  }
  
  
  if (getDungeon()->isSpace(nextRow, curCol) or getDungeon()->isGameObject(nextRow, curCol)) {
    // Move along row if possible
    move(curRow, curCol, nextRow, curCol);
  } else if (getDungeon()->isSpace(curRow, nextCol) or getDungeon()->isGameObject(curRow, nextCol)) {
    // Else move along column if possible
    move(curRow, curCol, curRow, nextCol);
  }
}
