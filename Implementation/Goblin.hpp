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
    
    Player* player = getDungeon()->getPlayer();
    
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*) player);
    } else {
      char maze[NUM_ROWS][NUM_COLS];
      for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
          maze[i][j] = ' ';
        }
      }
      
      vector<vector<int>> route;
      vector<int> step;
      int minDist = INT_MAX;
      
      testMove(maze, getRowPosition(), getColPosition(), player->getRowPosition(), player->getColPosition(), minDist, 0, route, step);
      if (step.size() != 0) {
        move(getRowPosition(), getColPosition(), step[0], step[1]);
      }
    }
  }
  
  bool isValid(char maze[][NUM_COLS], int startRow, int startCol) {
    return startRow != 0 and startRow != NUM_ROWS and startCol != 0 and startCol != NUM_COLS
    and maze[startRow][startCol] != 'o' and !getDungeon()->isWall(startRow, startCol) and !getDungeon()->isMonster(startRow, startCol);
  }
  
  void testMove(char maze[][NUM_COLS], int startRow, int startCol, int endRow, int endCol, int &minDist, int dist, vector<vector<int>> &route, vector<int> &step) {
    if (dist > 10 or dist > minDist)
      return;
    
    if (startRow == endRow and startCol == endCol) {
      if (dist < minDist) {
        minDist = dist;
        step = route[1];
      }
      return;
    }
    
    maze[startRow][startCol] = 'o';
    route.push_back({startRow, startCol});
    
    if (isValid(maze, startRow+1, startCol))
      testMove(maze, startRow+1, startCol, endRow, endCol, minDist, dist+1, route, step);
    if (isValid(maze, startRow-1, startCol))
      testMove(maze, startRow-1, startCol, endRow, endCol, minDist, dist+1, route, step);
    if (isValid(maze, startRow, startCol+1))
      testMove(maze, startRow, startCol+1, endRow, endCol, minDist, dist+1, route, step);
    if (isValid(maze, startRow, startCol-1))
      testMove(maze, startRow, startCol-1, endRow, endCol, minDist, dist+1, route, step);
    
    maze[startRow][startCol] = ' ';
    route.pop_back();
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
