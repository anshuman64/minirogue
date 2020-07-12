//
//  Player.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include "Actor.hpp"


class GameObject;
class Weapon;
class Scroll;

// Player class. Moves, takes action, and holds inventory
class Player : public Actor {
public:
  // Constructor
  Player(Dungeon* dungeon);
  ~Player();
  
  // Actions
  void calculateMove(char input);
  bool takeAction();
  void godMode();
  
  // Display
  void displayStats();
  void displayInventory();
  
private:
  bool                is_godMode;
  vector<GameObject*> m_inventory;
};

#endif /* Player_hpp */
