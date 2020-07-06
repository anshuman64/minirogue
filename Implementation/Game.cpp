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
      case 'c':
        m_dungeon->getPlayer()->godMode();
        break;
      case 'g':
        didPlayerMove = m_dungeon->getPlayer()->pickGameObject();
        break;
      case 'i':
        m_dungeon->getPlayer()->displayInventory();
        break;
      case 'e':
        m_dungeon->getPlayer()->displayWeapons();
        break;
      case 'h':
        displayHelp();
        break;
      case 'q':
        m_dungeon->endGame("Player quit the game.");
        break;
    }
    
    if (didPlayerMove) {
      m_dungeon->moveMonsters();
    }
  }
}

void Game::displayHelp() {
  clearScreen();
  
  cout << "Help - MiniRogue: Clear the dungeon and obtain the golden idol! Defeat monsters, find treasures, and descend deeper into the dungeon's depths." << endl << endl;
  
  cout << "Keyboard commands: " << endl;
  cout << "w,a,s,d. Move player" << endl;
  cout << "g. Pickup item" << endl;
  cout << "i. Display inventory" << endl;
  cout << "e. Equip weapon" << endl;
  cout << "s. Use scroll" << endl;
  cout << "c. Enter god mode" << endl;
  cout << "h. Display help" << endl;
  cout << "q. Quit game" << endl;
  
  cout << endl << "Press any key to continue.";
  
  getCharacter();
}
