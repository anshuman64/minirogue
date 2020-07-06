//
//  Scroll.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef Scroll_hpp
#define Scroll_hpp

#include <stdio.h>
#include <string>
#include "GameObject.hpp"

class Scroll : public GameObject {
public:
  Scroll(Dungeon* dungeon, string name, string description, string actionString) : GameObject(dungeon, '?', name, description, actionString) {}
  
  virtual void useScroll() = 0;
  
private:
  string m_actionString;
};

class TeleportationScroll : public Scroll {
public:
  TeleportationScroll(Dungeon* dungeon) : Scroll(dungeon, "Scroll of Teleportation", "Player is randomly moved to another place in the level", "You teleported! You feel your body wrenched in space and time.") {}
  
  virtual void useScroll() {
    Dungeon* dungeon = getDungeon();
    Player*  player  = dungeon->getPlayer();
    int nextRow = -1;
    int nextCol = -1;
    
    dungeon->generateRandomPosition(player, nextRow, nextCol);
    player->move(player->getRowPosition(), player->getColPosition(), nextRow, nextCol);
  }
};


class HealthScroll : public Scroll {
public:
  HealthScroll(Dungeon* dungeon) : Scroll(dungeon, "Scroll of Enhance Health", "Increase max HP", "Your max HP increases! You feel your heart beating stronger.") {}
  
  virtual void useScroll() {
    getDungeon()->getPlayer()->changeMaxHP(randInt(6)+3);
  }
};


class ArmorScroll : public Scroll {
public:
  ArmorScroll(Dungeon* dungeon) : Scroll(dungeon, "Scroll of Enhance Armor", "Increase armor rating", "Your armor increases! Your armor glows blue.") {}
  
  virtual void useScroll() {
    getDungeon()->getPlayer()->changeArmor(randInt(3)+1);
  }
};


class StrengthScroll : public Scroll {
public:
  StrengthScroll(Dungeon* dungeon) : Scroll(dungeon, "Scroll of Enhance Strength", "Increase strength rating", "Your strength increases! Your muscles bulge.") {}
  
  virtual void useScroll() {
    getDungeon()->getPlayer()->changeStrength(randInt(3)+1);
  }
};


class DexterityScroll : public Scroll {
public:
  DexterityScroll(Dungeon* dungeon) : Scroll(dungeon, "Scroll of Enhance Dexterity", "Increase dexterity rating", "Your dexterity increases! You feel like less of a klutz.") {}
  
  virtual void useScroll() {
    getDungeon()->getPlayer()->changeDexterity(1);
  }
};

#endif /* Scroll_hpp */
