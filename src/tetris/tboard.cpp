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

#include "tpoint.h"
#include "tboard.h"

const string tBoard::NAMES[7] = {   "-",
                                    "-",
                                    "block",
                                    "lslant",
                                    "rslant",
                                    "stick",
                                    "middle"
                                };

const string tBoard::OCCUPIED = "0";
const string tBoard::UNOCCUPIED = "-";

tBoard::tBoard()
{}

tBoard::~tBoard()
{}

void tBoard::clear()
{
   objects_.clear();
}

tObject tBoard::getObject(int id)
{
   tObject answer;
   for (int z = 0; z < objects_.size() ; z++)
   {
      if (objects_[z].id == id)
      {
         answer = objects_[z];
         break;
      }
   }
   return answer;
}

bool tBoard::doesObjectConflict(tObject thisObject)
{
   bool answer = false;
   //for every point in this object, compare it to every point in every other object
   for (int q = 0; q < thisObject.points_.size(); q++)
   {
      for (int z = 0; z < objects_.size() - 1; z++)
      {
         if (objects_[z].id != thisObject.id)
         {
            if (objects_[z].doesPointExist(thisObject.points_[q]) == true)
            {
               answer = true;
            }
         }
      }
   }
   return answer;
}

/** This will move an object if it can be moved. */
bool tBoard::moveObject(int objectId,int direction)
{
   bool answer = false;
   for (int x = 0; x < objects_.size(); x++)
   {
      if (objects_[x].id == objectId)
      {
         /*  Here's the deal, this might be slow, but it works great
         Create a copy of this object on the board, move it,
         if it conflicts with another other object, then allow the
         original object to be moved */

         tObject thisObject = objects_[x];
         thisObject.move(direction,tPoint(0,0),tPoint(BOARD_WIDTH,BOARD_HEIGHT));

         if (doesObjectConflict(thisObject))
            return false;

         //everything is fine so move it!
         answer = objects_[x].move(direction,tPoint(0,0),tPoint(BOARD_WIDTH,BOARD_HEIGHT));

         break;
      }
   }

   return answer;
}

/** Rotate the object left or right */
void tBoard::rotateObject(int objectId,int direction)
{
   //find the object we are looking for
   for (int x = 0; x < objects_.size(); x++)
   {
      if (objects_[x].id == objectId)
      {
         /* Rotate the object the number of times necessary */

         if (direction == RIGHT)
         {
            rotateObjectRight(objects_[x]);
         }
         else
         {
            //rotating right 3 times is the same as rotating left 1 time
            for (int i=0; i < 3; i++)
            {
               rotateObjectRight(objects_[x]);
            }
         }

         break;
      }
   }

}

void tBoard::rotateObjectRight(tObject& theObject)
{
   theObject.rotateRight();
}

bool tBoard::addObject(tObject newObject)
{
   bool answer;

   objects_.push_back(newObject);

   return answer;
}

/**
This function will build the board matrix out of all the objects.  If skipindex is != -1
it will skip that index when building the board
*/
vector<vector<string> > tBoard::getBoard(int excludeObjectId)
{

   vector<vector<string> > matrix;  //create default matrix

   vector<string> singleRow;
   //set matrix to all unoccupied
   for (int x = 0; x < BOARD_WIDTH; x++)
   {
      singleRow.clear();
      for (int y = 0; y < BOARD_HEIGHT; y++)
      {
         singleRow.push_back(UNOCCUPIED);
      }
      matrix.push_back(singleRow);
   }

   //now loop though each object, and each point in those objects
   for (int o = 0; o < objects_.size(); o++)
   {
      if (excludeObjectId != objects_[o].id)
      {
         for (int p = 0; p < objects_[o].points_.size(); p++)
         {
            matrix[objects_[o].points_[p].x_][objects_[o].points_[p].y_] = OCCUPIED /*itoa(objects_[o].id)*/;
         }
      }
   }

   return matrix;
}

/** Remove lines that are made and return how many were removed */
int tBoard::removeLines()
{
   int answer = 0;

   //cout << endl<< endl;
   //for each line on the board
   for (int y = 0; y < tBoard::BOARD_HEIGHT; y++)
   {
      //cout << "Working on line " << y << endl;
      int totalInThisRow = 0;

      //for each object on the board
      for (int o = 0; o < objects_.size(); o++)
      {
         //for each point in this object
         for (int p = 0; p < objects_[o].points_.size(); p++)
         {
            //if the point lies on this line, incriment
            if (objects_[o].points_[p].y_ == y)
            {
               totalInThisRow++;
               //cout << "found object" << endl;
            }
         }
      }

      //this row is full, take and loop through all those points and delete them
      if (totalInThisRow == 10)
      {
         //cout << "found complete row" << endl;
         //another line was found
         answer++;

         //delete the line by going through each object on the board
         for (int o = 0; o < objects_.size(); o++)
         {
            //for each point in this object
            for (int p = 0; p < objects_[o].points_.size(); p++)
            {
               if (objects_[o].points_[p].y_ == y)
               {
                  //cout << "Erasing: " << objects_[o].points_[p].x_ << endl;
                  objects_[o].points_.erase(objects_[o].points_.begin()+p);
                  //cout << objects_[o].points_.size() << " points in object now" << endl;
                  p--;
               }
               //else
               // cout << "y=" << objects_[o].points_[p].y_ << endl;

            }
         }

         //now I need to shift all other points above this line down 1
         for (int o = 0; o < objects_.size(); o++)
         {
            //for each point in this object
            for (int p = 0; p < objects_[o].points_.size(); p++)
            {
               if (objects_[o].points_[p].y_ < y)
               {
                  objects_[o].points_[p].y_++;
               }
            }
         }
      }
   }

   return answer;
}
