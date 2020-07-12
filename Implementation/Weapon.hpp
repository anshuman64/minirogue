//
//  Weapon.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Weapon_hpp
#define Weapon_hpp

#include <stdio.h>
#include <string>
#include "GameObject.hpp"


class Weapon : public GameObject {
public:
  // ******************************
  // * Constructor
  // ******************************
  
  Weapon(Dungeon* dungeon, string name, int damage, int dexBonus, string actionString) : GameObject(dungeon, ')', name, "Damage: +" + to_string(damage) + ", Dexterity: +" + to_string(dexBonus), actionString), m_damage(damage), m_dexBonus(dexBonus) {}
  
  
  // ******************************
  // * Accessors
  // ******************************
  
  int getDamage() {
    return m_damage;
  }
  
  int getDexBonus() {
    return m_dexBonus;
  }
  
  virtual bool isMagicFangs() {
    return false;
  }
  
private:
  int    m_damage;
  int    m_dexBonus;
};


class ShortSword : public Weapon {
public:
  ShortSword(Dungeon* dungeon) : Weapon(dungeon, "Short Sword", 2, 0, "slashes") {}
};

class Mace : public Weapon {
public:
  Mace(Dungeon* dungeon) : Weapon(dungeon, "Mace", 2, 0, "swings") {}
};

class LongSword : public Weapon {
public:
  LongSword(Dungeon* dungeon) : Weapon(dungeon, "Long Sword", 4, 2, "swings") {}
};

class MagicAxe : public Weapon {
public:
  MagicAxe(Dungeon* dungeon) : Weapon(dungeon, "Magic Axe", 5, 5, "chops") {}
};

class MagicFangs : public Weapon {
public:
  MagicFangs(Dungeon* dungeon) : Weapon(dungeon, "Magic Fangs", 3, 2, "strikes") {}
  
  virtual bool isMagicFangs() {
    return true;
  }
};

#endif /* Weapon_hpp */
