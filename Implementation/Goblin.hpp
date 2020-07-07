//
//  Goblin.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/6/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Goblin_hpp
#define Goblin_hpp

#include <stdio.h>
 
#include "Monster.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"


class Goblin : public Monster {
public:
  // ******************************
  // * Constructor
  // ******************************
  
  Goblin(Dungeon* dungeon) : Monster(dungeon, 'G', "Goblin", randInt(6)+15, 3, 1, 1) {
    setWeapon(new ShortSword(dungeon));
  }

  ~Goblin() {
    delete getWeapon();
  }
  
  
  // ******************************
  // * Actions
  // ******************************
  
  // TODO: update Goblin move function
  void calculateMove() {
    if (checkIsAsleep()) {
      return;
    }
    
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*)getDungeon()->getPlayer());
    } else if (stepsToPlayer() <= 5) {
      attemptMove();
    }
  }

  GameObject* dropGameObject() {
    if (!isOverGameObject() and trueWithProbability(0.3333)) {
      Weapon* droppedObject = trueWithProbability(0.5) ? (Weapon*)(new MagicAxe(getDungeon())) : (Weapon*)(new MagicFangs(getDungeon()));
      return droppedObject; // deleted in Dungeon
    }
    
    return nullptr;
  }
  
private:
  
};

#endif /* Goblin_hpp */
