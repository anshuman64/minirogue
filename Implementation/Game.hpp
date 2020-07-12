//
//  Game.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>


class Dungeon;


// Game controller
class Game {
public:
  // Constructor
  Game();
  ~Game();
  
  void play();
  
private:
  Dungeon* m_dungeon;
  
  void displayHelp();
};

#endif /* Game_hpp */
