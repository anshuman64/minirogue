//
//  Game.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include <iostream>
#include "Game.hpp"
#include "Dungeon.hpp"
#include "Player.hpp"
#include "utilities.hpp"

using namespace std;


// ******************************
// * Constructor
// ******************************

Game::Game() {
  m_dungeon = new Dungeon(); // deleted in ~Game
}

Game::~Game() {
  delete m_dungeon;
}


// ******************************
// * Start/Stop Game
// ******************************

// Takes keyboard input and routes to appropriate function
void Game::play() {
  char input;
  
  // Start game by displaying help
  displayHelp();
  
  while (true) {
    // Output game onto terminal
    m_dungeon->displayLevel();
    m_dungeon->getPlayer()->displayStats();
    m_dungeon->displayActions();
    
    // Quit game if game over
    if (m_dungeon->isGameOver()) {
      exit(-1);
    }
    
    // Get keyboard command
    cout << "Enter command (or 'h' for help): ";
    input = getCharacter();
    cout << endl << endl;
    
    // Bool to determine if monsters should move
    bool didPlayerMove = false;
    
    switch(input) {
      case ARROW_LEFT:
      case ARROW_RIGHT:
      case ARROW_UP:
      case ARROW_DOWN:
        // Attempt to move player in direction
        m_dungeon->getPlayer()->calculateMove(input);
        didPlayerMove = true;
        break;
      case 'f':
        // Attempt to pick up object or descend stairs
        didPlayerMove = m_dungeon->getPlayer()->takeAction();
        break;
      case 'e':
        // Display inventory
        m_dungeon->getPlayer()->displayInventory();
        break;
      case 'h':
        // Display help
        displayHelp();
        break;
      case 'q':
        // Attempt to quit game
        cout << "Re-enter 'q' to quit or any other key to continue: ";
        input = getCharacter();
        if (input == 'q') {
          m_dungeon->endGame("Player quit the game.");
        }
        break;
      case 'r':
        // Attempt to restart game
        cout << "Re-enter 'r' to restart or any other key to continue: ";
        input = getCharacter();
        if (input == 'r') {
          m_dungeon->~Dungeon();
          m_dungeon = new Dungeon();
          play();
        }
        break;
      case 'c':
        // Cheat mode by maxing all stats
        m_dungeon->getPlayer()->godMode();
        break;
    }
    
    // If player made a move, move monsters
    if (didPlayerMove) {
      m_dungeon->moveMonsters();
    }
  }
}

// Displays game objective, controls, and symbols
void Game::displayHelp() {
  clearScreen();
  
  cout << "**********************************************************************" << endl;
  cout << "*                                                                    *" << endl;
  cout << "*  **     **  ****  **   **  ****  ****  *****  *****  **  **  ****  *" << endl;
  cout << "*  **** ****   **   ***  **   **   ** *  ** **  **     **  **  **    *" << endl;
  cout << "*  ** *** **   **   ** * **   **   ***   ** **  ** **  **  **  ****  *" << endl;
  cout << "*  **  *  **   **   **  ***   **   ** *  ** **  **  *  ******  **    *" << endl;
  cout << "*  **     **  ****  **   **  ****  **  * *****  *****  ******  ****  *" << endl;
  cout << "*                                                                    *" << endl;
  cout << "**********************************************************************" << endl;

  cout << endl << "MiniRogue: Clear the dungeon and obtain the golden idol! Defeat monsters, find treasures, and descend deeper into the dungeon's depths." << endl << endl;
  
  cout << "Keyboard Commands:" << endl;
  cout << "w,a,s,d. Move player" << endl;
  cout << "e. Display inventory" << endl;
  cout << "f. Take action" << endl;
  cout << "h. Display help" << endl;
  cout << "q. Quit game" << endl;
  cout << "r. Restart game" << endl;
  cout << "c. Enter god mode" << endl;
  cout << endl;
  cout << "Game Symbols:" << endl;
  cout << "@. Player" << endl;
  cout << "B,D,G,S. Monsters" << endl;
  cout << "). Weapon" << endl;
  cout << "?. Scroll" << endl;
  cout << ">. Stairs" << endl;
  cout << "&. Golden Idol" << endl;
  
  cout << endl << "Press any key to continue: " << endl;
  
  getCharacter();
}
