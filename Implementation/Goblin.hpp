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
  
  void calculateMove() {
    if (checkIsAsleep()) {
      return;
    }
    
    Player* player = getDungeon()->getPlayer();
    
    // If next to Player, attack
    if (stepsToPlayer() == 1) {
      attack(this, (Actor*) player);
    } else {
      // Generate char maze for breadth-first search
      char maze[NUM_ROWS][NUM_COLS];
      for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLS; j++) {
          maze[i][j] = ' ';
        }
      }
      
      vector<vector<int>> route; // stores route to Player
      vector<int> step; // stores first step towards Player
      int minDist = INT_MAX; // stores length of least-distance path to Player
      
      // Recursively finds shortest path to Player
      pathToPlayer(maze, getRowPosition(), getColPosition(), player->getRowPosition(), player->getColPosition(), minDist, 0, route, step);
      
      // If step was populated with first step towards Player, move in that direction
      if (step.size() != 0) {
        move(getRowPosition(), getColPosition(), step[0], step[1]);
      }
    }
  }
  
  // Determines if step is valid based on if step is on border of dungeon, if step has been visited before, if step isWall, or if step isMonster
  bool isValid(char maze[][NUM_COLS], int startRow, int startCol) {
    return startRow != 0 and startRow != NUM_ROWS and startCol != 0 and startCol != NUM_COLS
    and maze[startRow][startCol] != 'o' and !getDungeon()->isWall(startRow, startCol) and !getDungeon()->isMonster(startRow, startCol);
  }
  
  void pathToPlayer(char maze[][NUM_COLS], int startRow, int startCol, int endRow, int endCol, int &minDist, int dist, vector<vector<int>> &route, vector<int> &step) {
    // If over Goblin's smell range or longer route than shortest path thus far, return
    if (dist > GOBLIN_SMELL_RANGE or dist > minDist) {
      return;
    }
      
    // If reached Player...
    if (startRow == endRow and startCol == endCol) {
      if (dist < minDist) {
        // Update shortest path if shorter
        minDist = dist;
        step = route[1];
      }
      
      return;
    }
    
    // Mark current cell as visited
    maze[startRow][startCol] = 'o';
    route.push_back({startRow, startCol});
    
    // Recursively traverse in each direction
    if (isValid(maze, startRow+1, startCol)) {
      pathToPlayer(maze, startRow+1, startCol, endRow, endCol, minDist, dist+1, route, step);
    }
      
    if (isValid(maze, startRow-1, startCol)) {
      pathToPlayer(maze, startRow-1, startCol, endRow, endCol, minDist, dist+1, route, step);
    }
      
    if (isValid(maze, startRow, startCol+1)) {
      pathToPlayer(maze, startRow, startCol+1, endRow, endCol, minDist, dist+1, route, step);
    }
      
    if (isValid(maze, startRow, startCol-1)) {
      pathToPlayer(maze, startRow, startCol-1, endRow, endCol, minDist, dist+1, route, step);
    }
      
    // Back-track from current cell if not part of the solution path
    maze[startRow][startCol] = ' ';
    route.pop_back();
  }

  // Randomly dropGameObject
  GameObject* dropGameObject() {
    if (!isOverGameObject() and trueWithProbability(0.3333)) {
      Weapon* droppedObject = trueWithProbability(0.5) ? (Weapon*)(new MagicAxe(getDungeon())) : (Weapon*)(new MagicFangs(getDungeon()));
      return droppedObject; // deleted in Dungeon
    }
    
    return nullptr;
  }
  
};

#endif /* Goblin_hpp */
