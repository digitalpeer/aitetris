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

#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include "tobject.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;

class tPoint;

/**
The board is the center of everything.  It accepts commands to manipulate it, it contains tObjects,
and it is what you query for the board status.
*/
class tBoard
{
public:
   enum { BOARD_WIDTH = 10, ///< The default board width
          BOARD_HEIGHT = 15,   ///< The default board height
          LEFT = 3,      ///< Direction left for movement
          RIGHT = 1,     ///< Direction right for movement
          DOWN = 2    ///< Direction down for movement
        };

   /** String names of all object types */
   static const string NAMES[7];

   /** An occupied block on the board */
   static const string OCCUPIED;

   /** An unoccupied block on the board */
   static const string UNOCCUPIED;


   tBoard();
   ~tBoard();

   /** Move an object on this board one space in the given direction */
   bool moveObject(int objectId,int direction);

   /** Rotate the object left or right */
   void rotateObject(int objectId,int direction);

   /** Grab the board */
   vector<vector<string> > getBoard(int excludeObjectId);

   /** Add an object to the board */
   bool addObject(tObject newObject);

   /** Test to see if the given object conflicts with any other object on the board */
   bool doesObjectConflict(tObject thisObject);

   tObject getObject(int id);

   /** Clear the board to a starting state */
   void clear();

   int removeLines();

private:

   /** List of all objects on the board */
   vector<tObject> objects_;

   /** Helper function to the rotateObject function */
   void rotateObjectRight(tObject& theObject);
};


#endif
