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


class Bogeyman : public Monster {
public:
  // Constructor
  Bogeyman(Dungeon* dungeon);
  ~Bogeyman();
  
  // Actions
  virtual void calculateMove();
  
private:
  
};

#endif /* Bogeyman_hpp */
