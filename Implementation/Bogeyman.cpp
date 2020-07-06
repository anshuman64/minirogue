//
//  Bogeyman.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include "Bogeyman.hpp"
#include "Weapon.hpp"
#include "utilities.hpp"



// ******************************
// * Constructor
// ******************************

Bogeyman::Bogeyman(Dungeon* dungeon) : Monster(dungeon, 'B', "Bogeyman", randInt(6)+5, randInt(2)+2, randInt(2)+2, 2) {
  setWeapon(new Weapon(dungeon, "Short Sword", 2, 0, "slashes"));
}

Bogeyman::~Bogeyman() {
  delete getWeapon();
}


// ******************************
// * Actions
// ******************************

void Bogeyman::calculateMove() {
  if (stepsToPlayer() == 1) {
    attack(this, (Actor*)getDungeon()->getPlayer());
  } else if (stepsToPlayer() <= 5) {
    attemptMove();
  }
}
