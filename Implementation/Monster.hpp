//
//  Monster.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Monster_hpp
#define Monster_hpp

#include <stdio.h>
#include "Actor.hpp"


class Monster : public Actor {
public:
  // Constructor
  Monster(Dungeon* dungeon, char symbol, string name, int hp, int armor, int strength, int dexterity);
  virtual ~Monster() {}
  
  // Actions
  virtual void calculateMove() = 0;
  void         attemptMove();
  int          stepsToPlayer();
  
private:
  // Helpers
  int rowsToPlayer();
  int colsToPlayer();
};

#endif /* Monster_hpp */
