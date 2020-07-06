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
  // Constructor
  Weapon(Dungeon* dungeon, string name, int damage, int dexBonus, string actionString) : GameObject(dungeon, ')', name, "Damage: +" + to_string(damage) + ", Dexterity: +" + to_string(dexBonus)), m_damage(damage), m_dexBonus(dexBonus), m_actionString(actionString) {}
  
  // Accessors
  int getDamage() {
    return m_damage;
  }
  
  int getDexBonus() {
    return m_dexBonus;
  }
  
  string getActionString() {
    return m_actionString;
  }
  
private:
  int m_damage;
  int m_dexBonus;
  string m_actionString;
};

#endif /* Weapon_hpp */
