//
//  Player.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include "Player.hpp"
#include "Weapon.hpp"
#include "Scroll.hpp"
#include "utilities.hpp"


// ******************************
// * Constructor
// ******************************
Player::Player(Dungeon* dungeon) : Actor(dungeon, '@', "Player", 20, 2, 2, 2), is_godMode(false) {
  Weapon* startWeapon = new ShortSword(dungeon); // deleted in ~Actor
  m_inventory.push_back(startWeapon);
  setWeapon(startWeapon);
}

Player::~Player() {
  // Delete all items in inventory
  while (!m_inventory.empty()) {
    Object* toDelete = m_inventory.back();
    m_inventory.pop_back();
    delete toDelete;
  }
}

// ******************************
// * Actions
// ******************************

void Player::calculateMove(char input) {
  // If asleep, do nothing and end turn
  if (checkIsAsleep()) {
    return;
  }
  
  // Regain hit points
  if (trueWithProbability(0.1)) {
    changeHP(1);
  }
  
  int curRow = getRowPosition();
  int curCol = getColPosition();
  int nextRow = curRow;
  int nextCol = curCol;
  string directionString;
  
  // Determine next cell Player is trying to move
  switch (input) {
    case ARROW_LEFT:
      nextCol--;
      directionString = "left";
      break;
    case ARROW_RIGHT:
      nextCol++;
      directionString = "right";
      break;
    case ARROW_UP:
      nextRow--;
      directionString = "up";
      break;
    case ARROW_DOWN:
      nextRow++;
      directionString = "down";
      break;
  }
  
  // If next cell isWall, block Player
  if (getDungeon()->isWall(nextRow, nextCol)) {
    getDungeon()->addAction("Player tried moving " + directionString + " but was blocked by a wall.");
  }
  
  // If next cell isSpace, move Player to space
  else if (getDungeon()->isSpace(nextRow, nextCol)) {
    move(curRow, curCol, nextRow, nextCol);
    getDungeon()->addAction("Player moved " + directionString + ".");
  }
  
  // If next cell isMonster, attack Monster
  else if (getDungeon()->isMonster(nextRow, nextCol)) {
    attack(this, (Actor*)getDungeon()->getObject(nextRow, nextCol));
  }
  
  // If next cell isGameObject, move Player onto GameObject
  else if (getDungeon()->isGameObject(nextRow, nextCol)) {
    move(curRow, curCol, nextRow, nextCol);
    getDungeon()->addAction("Player moved " + directionString + " onto " + getOverGameObject()->getName() + ". Press 'f' to take action.");
  }
}

// Return true if item picked up to set didPlayerMove in Game.cpp
bool Player::takeAction() {
  GameObject* gameObject = getOverGameObject();
  
  // If not over a GameObject, do nothing
  if (gameObject == nullptr) {
    getDungeon()->addAction("No action to take.");
    return false;
  }
  
  // If asleep, do nothing and end turn
  if (checkIsAsleep()) {
    return true;
  }
  
  // If over Golden Idol...
  if (gameObject->isGoldenIdol()) {
    // If monsters are remaining, prompt user to defeat all monsters
    if (getDungeon()->isMonstersRemaining()) {
      getDungeon()->addAction("Defeat all monsters on the level to pick up the Golden Idol.");
      return false;
    } else {
      // Else, Player wins game
      getDungeon()->endGame("Player picked up the Golden Idol. You win!");
      return false;
    }
  }
  
  // If over Stairs, descend Stairs to next level of dungeon
  if (getOverGameObject()->isStairs()) {
    setOverGameObject(nullptr);
    getDungeon()->addAction("Player moved to next level of dungeon!");
    getDungeon()->nextLevel();
    return false;
  }
  
  if (m_inventory.size() > 25) {
    getDungeon()->addAction("Inventory full.");
    return false;
  }
  
  // Else, GameObject is an item to be picked up
  m_inventory.push_back(gameObject);
  setOverGameObject(nullptr);
  getDungeon()->pickUpGameObject(gameObject);
  getDungeon()->addAction("Player picked up a " + gameObject->getName() + ".");
  return true;
}

// Max all stats to allow Player to clear the dungeon
void Player::godMode() {
  if (is_godMode) {
    getDungeon()->addAction("God Mode already activated.");
    return;
  }
  
  changeMaxHP(MAX_STATS);
  changeHP(MAX_STATS);
  changeArmor(MAX_STATS);
  changeStrength(MAX_STATS);
  changeDexterity(MAX_STATS);
  getDungeon()->addAction("Player feels the strength of the Almighty. All stats are maxed!");
  is_godMode = true;
}

// ******************************
// * Display
// ******************************

void Player::displayStats() {
  cout << "Dungeon Level: " << getDungeon()->getLevel() << " | ";
  cout << "HP: " << getHP() << "/" << getMaxHP() << " | ";
  cout << "Armor: " << getArmor() << " | ";
  cout << "Strength: " << getStrength() << "(+" << getWeapon()->getDamage() << ") | ";
  cout << "Dexterity: " << getDexterity() << "(+" << getWeapon()->getDexBonus() << ")";
  cout << endl << endl;
}

void Player::displayInventory() {
  clearScreen();
  
  // Prints inventory
  cout << "Inventory:" << endl;

  char letterIndex = 'a';
  for (int i = 0; i < m_inventory.size(); i++) {
    cout << (char)(letterIndex + i) << ". " << m_inventory[i]->getName() << " - " << m_inventory[i]->getDescription() << endl;
  }
  
  // Get input from Player to equip weapon, use scroll, or exit inventory
  cout << endl << "Press letter to equip weapon / use scroll or any other key to continue." << endl;
  
  char input = getCharacter();
  int inputIndex = input - letterIndex;
  
  // If input is in range of inventory slots...
  if (inputIndex < m_inventory.size()) {
    if (m_inventory[inputIndex]->isWeapon()) {
      // ...and isWeapon, equip weapon
      setWeapon((Weapon*)m_inventory[inputIndex]);
      getDungeon()->addAction("Player equipped " + m_inventory[inputIndex]->getName() + ".");
    } else if (m_inventory[inputIndex]->isScroll()) {
      // ...and isScroll, use scroll
      Scroll* scrollToUse = (Scroll*)m_inventory[inputIndex];
      scrollToUse->useScroll();
      getDungeon()->addAction(scrollToUse->getActionString());
      
      m_inventory.erase(m_inventory.begin() + inputIndex);
      delete scrollToUse;
    }
  }
}
