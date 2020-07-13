//
//  Dragon.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/6/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Dragon_hpp
#define Dragon_hpp

#include <stdio.h>

#include "Monster.hpp"
#include "Weapon.hpp"
#include "Scroll.hpp"
#include "utilities.hpp"


class Dragon : public Monster {
public:
  // ******************************
  // * Constructor
  // ******************************
  
  Dragon(Dungeon* dungeon) : Monster(dungeon, 'D', "Dragon", randInt(6)+20, 4, 4, 4) {
    setWeapon(new LongSword(dungeon));
  }

  ~Dragon() {}
  
  
  // ******************************
  // * Actions
  // ******************************
  
  void calculateMove() {
    if (checkIsAsleep()) {
      return;
    }
    
    // Recover HP
    if (trueWithProbability(0.1)) {
      changeHP(1);
    }
    
    // If next to Player, attack
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*)getDungeon()->getPlayer());
    }
  }

  // Randomly dropGameObject
  GameObject* dropGameObject() {
    if (!isOverGameObject()) {
      switch (randInt(5)) {
        case 0:
          return new TeleportationScroll(getDungeon()); // deleted in Dungeon::resetLevel or Dungeon::pickUpGameObject
          break;
        case 1:
          return new HealthScroll(getDungeon());
          break;
        case 2:
          return new ArmorScroll(getDungeon());
          break;
        case 3:
          return new StrengthScroll(getDungeon());
          break;
        case 4:
          return new DexterityScroll(getDungeon());
          break;
      }
    }
    
    return nullptr;
  }
  
private:
  
};

#endif /* Dragon_hpp */
