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
#include "utilities.hpp"


class Dragon : public Monster {
public:
  // ******************************
  // * Constructor
  // ******************************
  
  Dragon(Dungeon* dungeon) : Monster(dungeon, 'D', "Dragon", randInt(6)+20, 4, 4, 4) {
    setWeapon(new LongSword(dungeon));
  }

  ~Dragon() {
    delete getWeapon();
  }
  
  
  // ******************************
  // * Actions
  // ******************************
  void calculateMove() {
    if (trueWithProbability(0.1)) {
      changeHP(1);
    }
    
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*)getDungeon()->getPlayer());
    }
  }

  GameObject* dropGameObject() {
    if (!isOverGameObject()) {
      return new MagicFangs(getDungeon()); // deleted in Dungeon
    }
    
    return nullptr;
  }
  
private:
  
};

#endif /* Dragon_hpp */
