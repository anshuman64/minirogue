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
  Weapon* startWeapon = new ShortSword(dungeon);
  m_inventory.push_back(startWeapon);
  m_weapons.push_back(startWeapon);
  setWeapon(startWeapon);
}

Player::~Player() {
  delete getWeapon();
  
  for (int i = 0; i < m_weapons.size(); i++) {
    delete m_weapons[i];
  }
}

// ******************************
// * Actions
// ******************************

void Player::calculateMove(char input) {
  // If asleep
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
  
  if (getDungeon()->isWall(nextRow, nextCol)) {
    getDungeon()->addAction("Player tried moving " + directionString + " but was blocked by a wall.");
  }
  
  else if (getDungeon()->isSpace(nextRow, nextCol)) {
    move(curRow, curCol, nextRow, nextCol);
    getDungeon()->addAction("Player moved " + directionString + ".");
  }
  
  else if (getDungeon()->isMonster(nextRow, nextCol)) {
    attack(this, (Actor*)getDungeon()->getObject(nextRow, nextCol));
  }
  
  else if (getDungeon()->isGameObject(nextRow, nextCol)) {
    move(curRow, curCol, nextRow, nextCol);
    getDungeon()->addAction("Player moved " + directionString + " onto " + getOverGameObject()->getName() + ".");
  }
}

// Return true if item picked up to set didPlayerMove in Game.cpp
bool Player::pickGameObject() {
  GameObject* gameObject = getOverGameObject();
  
  if (gameObject == nullptr) {
    getDungeon()->addAction("No item to pick up.");
    return false;
  }
  
  if (isAsleep()) {
    getDungeon()->endGame("Player is asleep and cannot move.");
    changeAsleep(-1);
    return true;
  }
  
  if (gameObject->isGoldenIdol()) {
    if (getDungeon()->isMonstersRemaining()) {
      getDungeon()->addAction("Defeat all monsters on the level to pick up the Golden Idol.");
      return false;
    } else {
      getDungeon()->endGame("Player picked up the Golden Idol. You win!");
      return false;
    }
  }
  
  if (m_inventory.size() > 25) {
    getDungeon()->addAction("Inventory full.");
    return false;
  }
  
  m_inventory.push_back(gameObject);
  if (gameObject->isWeapon()) {
    m_weapons.push_back((Weapon*)gameObject);
  } else if (gameObject->isScroll()) {
    m_scrolls.push_back((Scroll*)gameObject);
  }
  
  setOverGameObject(nullptr);
  getDungeon()->addAction("Player picked up a " + gameObject->getName() + ".");
  return true;
}

void Player::descendStairs() {
  if (getOverGameObject() == nullptr or !getOverGameObject()->isStairs()) {
    getDungeon()->addAction("No stairs to descend.");
    return;
  }

  setOverGameObject(nullptr);
  getDungeon()->addAction("Player moved to next level of dungeon!");
  getDungeon()->nextLevel();
}

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
  getDungeon()->addAction("Player feels the strength of the Almighty.");
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
  
  cout << "Inventory:" << endl;
  
  char letterIndex = 'a';
  for (int i = 0; i < m_inventory.size(); i++) {
    cout << (char)(letterIndex + i) << ". " << m_inventory[i]->getName() << " - " << m_inventory[i]->getDescription() << endl;
  }
  
  cout << endl << "Press letter to equip weapon / use scroll or any other key to continue." << endl;
  
  char input = getCharacter();
  int inputIndex = input - letterIndex;
  
  if (inputIndex < m_inventory.size()) {
    if (m_inventory[inputIndex]->isWeapon()) {
      setWeapon((Weapon*)m_inventory[inputIndex]);
      getDungeon()->addAction("Player equipped " + m_inventory[inputIndex]->getName() + ".");
    } else if (m_inventory[inputIndex]->isScroll()) {
      Scroll* scrollToUse = (Scroll*)m_inventory[inputIndex];
      scrollToUse->useScroll();
      getDungeon()->addAction(scrollToUse->getActionString());
      
      m_inventory.erase(m_inventory.begin() + inputIndex);
      delete scrollToUse;
    }
  }
}
