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

  ~Snakewoman() {
    delete getWeapon();
  }
  
  
  // ******************************
  // * Actions
  // ******************************
  void calculateMove() {
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*)getDungeon()->getPlayer());
    } else if (stepsToPlayer() <= 3) {
      attemptMove();
    }
  }

  GameObject* dropGameObject() {
    if (!isOverGameObject() and trueWithProbability(0.3333)) {
      return new MagicFangs(getDungeon()); // deleted in Dungeon
    }
    
    return nullptr;
  }
  
private:
  
};

#endif /* Snakewoman_hpp */
