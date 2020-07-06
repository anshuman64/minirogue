//
//  Bogeyman.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Bogeyman_hpp
#define Bogeyman_hpp

#include <stdio.h>

#include "Monster.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"


class Bogeyman : public Monster {
public:
  // ******************************
  // * Constructor
  // ******************************
  
  Bogeyman(Dungeon* dungeon) : Monster(dungeon, 'B', "Bogeyman", randInt(6)+5, randInt(2)+2, randInt(2)+2, 2) {
    setWeapon(new ShortSword(dungeon));
  }

  ~Bogeyman() {
    delete getWeapon();
  }
  
  
  // ******************************
  // * Actions
  // ******************************
  void calculateMove() {
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*)getDungeon()->getPlayer());
    } else if (stepsToPlayer() <= 5) {
      attemptMove();
    }
  }

  GameObject* dropGameObject() {
    if (!isOverGameObject() and trueWithProbability(0.1)) {
      return new MagicAxe(getDungeon()); // deleted in Dungeon
    }
    
    return nullptr;
  }
  
private:
  
};

#endif /* Bogeyman_hpp */
