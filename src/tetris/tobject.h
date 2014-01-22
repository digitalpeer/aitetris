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

#ifndef TETRIS_OBJECT_H
#define TETRIS_OBJECT_H

#include <vector>
#include <string>
#include <iostream>
using namespace std;

class tPoint;

/*
An object is just a set of points and nothing more.
These points represent it's location on the board.
An objects top left corner (0,0) is equal to the board's top left
corner (0,0).
*/
class tObject
{
public:
   enum {   OBJECT_WIDTH = 4,
            OBJECT_HEIGHT = 4,
            BOARD = 1,
            BLOCK = 2,
            LSLANT = 3,
            RSLANT = 4,
            STICK = 5,
            MIDDLE = 6
        };

   tObject(int theId = -1);

   tObject(vector<tPoint> points, int theId = -1);

   ~tObject();

   /** The ID of the object */
   int id;

   string name;

   /** Shift all the points in the object as long as the object stays
   within min and max */
   bool move(int direction, tPoint min, tPoint max);

   /** get the matrix of the object */
   vector<vector<string> > getObject();

   /** Add a point to the object */
   void addPoint(tPoint newPoint);

   /** Clear the object to a "null" state */
   void clear();

   /** Check to see if thePoint exists in this object */
   bool doesPointExist(tPoint thePoint);

   /** Vector of all points in the object */
   vector<tPoint> points_;

   void rotateRight();

};

#endif
