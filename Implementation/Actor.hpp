//
//  Actor.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Actor_hpp
#define Actor_hpp

#include <stdio.h>
#include "Object.hpp"


class Weapon;
class GameObject;

// Parent class for Player or Monsters. Includes methods for move and combat
class Actor : public Object {
public:
  // Constructor
  Actor (Dungeon* dungeon, char symbol, string name, int hp, int armor, int strength, int dexterity);
  virtual ~Actor() {}
  
  // Accessors - Stats
  int getHP();
  int getMaxHP();
  int getArmor();
  int getStrength();
  int getDexterity();
  
  // Accessors - Other
  Weapon*     getWeapon();
  GameObject* getOverGameObject();
  
  // Accessors - Booleans
  bool isOverGameObject();
  bool isAsleep();
  
  // Mutators - Stats
  void changeHP(int delta);
  void changeMaxHP(int delta);
  void changeArmor(int delta);
  void changeStrength(int delta);
  void changeDexterity(int delta);
  void changeAsleep(int delta);
  
  // Mutators - Other
  void setWeapon(Weapon* weapon);
  void setOverGameObject(GameObject* gameObject);
  
  // Combat
  void move(int curRow, int curCol, int nextRow, int nextCol);
  void attack(Actor* attacker, Actor* defender);
  bool isHit(Actor* attacker, Actor* defender);
  void checkIsDead(Actor* actor);
  bool checkIsAsleep();
  
private:
  int m_hp;
  int m_maxHP;
  int m_armor;
  int m_strength;
  int m_dexterity;
  int m_turnsAsleep;
  
  Weapon*     m_weapon;
  GameObject* m_overGameObject;
};


#endif /* Actor_hpp */
