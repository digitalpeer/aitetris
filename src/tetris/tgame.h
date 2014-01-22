/*
AiTetris (http://www.digitalpeer.com/projects/aitetris/)
Copyright (C) 2002  Josh Henderson (digitalpeer)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "tpoint.h"
#include "tobject.h"
#include "tboard.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

/**
This is the actual tetris game. It contains a board, game state, generates random tObjects
for the tBoard, handles the controls from the user, and holds the default tObjects.

Here's the sequence of events:
   -# Load a board if necessary
   -# executeMoves and runGame as many times as necessary
   -# Save game if necessary
*/
class tGame
{
public:

   tGame();
   ~tGame();

   /** Rest the game and initialize it to a starting state */
   void reset();

   /** Move the active object to the left */
   bool moveLeft();

   /** Move the active object to the right */
   bool moveRight();

   /** Change/rotate the active object to the left */
   void changeLeft();

   /** Change/rotate the active object to the right */
   void changeRight();

   /** Load a game state file */
   bool loadGame(string fileName, bool showBoard = false);

   /** Run the game */
   bool runGame(bool hideOutput = true);

   /** Save the game state to a file */
   void saveGame(string fileName,bool hideOutput = true);

   /** Get the board matrix */
   vector<vector<string> > getBoardMatrix(int excludeObjectId=-99);

   /** Get the ID of the current object */
   int getCurrentObjectId();

   /** This is a special function used to get the lowest level of the current object */
   int getLowestBlock(int objectId);

private:

   /** A vector of the default pieces that will be played with */
   vector<tObject> defaultObjects;

   /** The board (which contains all the other objects) */
   tBoard board_;

   /** The current object that is being moved for this move */
   tObject currentObject;

   /** The generated object for the next move */
   tObject nextObject;

   /** Generate the default objects */
   void generateDefaultObjects();

   /** The number of times this board has been played */
   int numberOfRuns;

   /** The number of lines that have been made */
   int numberOfLines;

   /** Number of games played */
   int gamesPlayed;

   /** Is the board playable */
   bool playable;

   /** Settle the currentObject to the bottom of the board */
   void settleBoard();

   /** If there is a collesion between pieces then the game is over */
   bool isGameOver();

};

#endif
