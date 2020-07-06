//
//  Wall.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/2/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Wall_hpp
#define Wall_hpp

#include <stdio.h>


class Wall : public Object {
public: 
  Wall(Dungeon* dungeon, int rowPos, int colPos) : Object(dungeon, '#', "Wall", rowPos, colPos) {}
};

#endif /* Wall_hpp */
