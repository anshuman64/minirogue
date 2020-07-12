//
//  GameObject.hpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "Object.hpp"


class GameObject : public Object {
public:
  // ******************************
  // * Constructor
  // ******************************

  GameObject(Dungeon* dungeon, char symbol, string name) : Object(dungeon, symbol, name) {}
  
  // Constructor for weapons and scrolls
  GameObject(Dungeon* dungeon, char symbol, string name, string description, string actionString) : Object(dungeon, symbol, name), m_description(description), m_actionString(actionString) {}
  
  virtual ~GameObject() {}
  
  
  // ******************************
  // * Accessors - Booleans
  // ******************************
  bool isWeapon() {
    return getSymbol() == ')';
  }
  
  bool isScroll() {
    return getSymbol() == '?';
  }
  
  bool isGoldenIdol() {
    return getSymbol() == '&';
  }
  
  bool isStairs() {
    return getSymbol() == '>';
  }
  
  
  // ******************************
  // * Accessors - Other
  // ******************************
  string getDescription() {
    return m_description;
  }
  
  string getActionString() {
    return m_actionString;
  }
  
private:
  string m_description;
  string m_actionString;
};

#endif /* GameObject_hpp */
