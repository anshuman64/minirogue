//
//  Room.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/3/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Room_hpp
#define Room_hpp

#include <stdio.h>
#include "Object.hpp"


class Room : public Object {
public:
  Room(Dungeon* dungeon, int rowPos, int colPos) : Object(dungeon, ' ', "Room", rowPos, colPos) {}
};

#endif /* Room_hpp */
