//
//  Snakewoman.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/6/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Snakewoman_hpp
#define Snakewoman_hpp

#include <stdio.h>

#include "Monster.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"


class Snakewoman : public Monster {
public:
  // ******************************
  // * Constructor
  // ******************************
  
  Snakewoman(Dungeon* dungeon) : Monster(dungeon, 'S', "Snakewoman", randInt(4)+3, 2, 3, 3) {
    setWeapon(new MagicFangs(dungeon));
  }

  ~Snakewoman() {}
  
  
  // ******************************
  // * Actions
  // ******************************
  
  void calculateMove() {
    if (checkIsAsleep()) {
      return;
    }
    
    if (stepsToPlayer() == 1) {
      // If next to Player, attack
      attack(this, (Actor*)getDungeon()->getPlayer());
    } else if (stepsToPlayer() <= SNAKEWOMAN_SMELL_RANGE) {
      // Else if 3 steps from Player, attemptMove
      attemptMove();
    }
  }

  // Randomly dropGameObject
  GameObject* dropGameObject() {
    if (!isOverGameObject() and trueWithProbability(0.3333)) {
      return new MagicFangs(getDungeon()); // deleted in Dungeon::resetLevel or Dungeon::pickUpGameObject
    }
    
    return nullptr;
  }
  
};

#endif /* Snakewoman_hpp */
