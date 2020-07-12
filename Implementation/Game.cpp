//
//  Game.cpp
//  Project3
//
//  Created by Anshuman Dewangan on 7/1/20.
//  Copyright © 2020 Anshuman Dewangan. All rights reserved.
//

#include <iostream>
#include "Game.hpp"
#include "Player.hpp"
#include "utilities.hpp"

using namespace std;


// ******************************
// * Constructor
// ******************************

Game::Game() {
  m_dungeon = new Dungeon();
}

Game::~Game() {
  delete m_dungeon;
}


// ******************************
// * Start/Stop Game
// ******************************

void Game::play() {
  char input;
  
  displayHelp();
  
  while (true) {
    m_dungeon->displayLevel();
    m_dungeon->getPlayer()->displayStats();
    m_dungeon->displayActions();
    
    if (m_dungeon->isGameOver()) {
      exit(-1);
    }
    
    cout << "Enter command (or 'h' for help): ";
    input = getCharacter();
    cout << endl;
    
    bool didPlayerMove = false;
    switch(input) {
      case ARROW_LEFT:
      case ARROW_RIGHT:
      case ARROW_UP:
      case ARROW_DOWN:
        m_dungeon->getPlayer()->calculateMove(input);
        didPlayerMove = true;
        break;
      case 'f':
        didPlayerMove = m_dungeon->getPlayer()->takeAction();
        break;
      case 'e':
        m_dungeon->getPlayer()->displayInventory();
        break;
      case 'h':
        displayHelp();
        break;
      case 'q':
        
        m_dungeon->endGame("Player quit the game.");
        break;
      case 'c':
        m_dungeon->getPlayer()->godMode();
        break;
    }
    
    if (didPlayerMove) {
      m_dungeon->moveMonsters();
    }
  }
}

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
  
  cout << "Keyboard commands:" << "\t\t\t"  << "Game Symbols:" << endl;
  cout << "w,a,s,d. Move player" << "\t\t"  << "@. Player" << endl;
  cout << "e. Display inventory"  << "\t\t" << "B,D,G,S. Monsters" << endl;
  cout << "f. Take action"  << "\t\t\t\t\t" << "). Weapon" << endl;
  cout << "h. Display help"  << "\t\t\t\t " << "?. Scroll" << endl;
  cout << "q. Quit game"  << "\t\t\t\t\t\t" << ">. Stairs" << endl;
  cout << "c. Enter god mode"  << "\t\t\t " << "&. Golden Idol" << endl;
  
  cout << endl << "Press any key to continue: " << endl;
  
  getCharacter();
}
