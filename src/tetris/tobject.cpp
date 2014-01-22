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

#include "../util/util.h"

#include "tpoint.h"
#include "tboard.h"
#include "tobject.h"

tObject::tObject(int theId)
{
   id = theId;
}

tObject::tObject(vector<tPoint> points, int theId)
{
   id = theId;
   points_ = points;
}

tObject::~tObject()
{}

bool tObject::doesPointExist(tPoint thePoint)
{
   bool answer = false;
   for (int x = 0; x < points_.size(); x++)
   {
      if (points_[x].x_ == thePoint.x_ && points_[x].y_ == thePoint.y_)
      {
         answer = true;
         break;
      }
   }
   return answer;
}

/**
Take and shift all the points in the direction needed.  If it was an illegal move
then false is returned
*/
bool tObject::move(int direction, tPoint min, tPoint max)
{
   bool answer = true;
   for (int p = 0; p < points_.size(); p++)
   {
      switch (direction)
      {
      case tBoard::DOWN:
         if (points_[p].y_+2 > max.y_)
         {
            return false;
         }

         break;
      case tBoard::LEFT:
         if (points_[p].x_-2 < min.x_)
         {
            return false;
         }

         break;
      case tBoard::RIGHT:
         if (points_[p].x_+2 > max.x_)
         {
            return false;
         }

         break;
      }

   }

   for (int p = 0; p < points_.size() && answer; p++)
   {
      switch (direction)
      {
      case tBoard::DOWN:
         points_[p].y_ = points_[p].y_ + 1;
         break;
      case tBoard::LEFT:
         points_[p].x_ = points_[p].x_ - 1;
         break;
      case tBoard::RIGHT:
         points_[p].x_ = points_[p].x_ + 1;
         break;
      }
   }

   return answer;
}

void tObject::clear()
{
   points_.clear();
}

void tObject::addPoint(tPoint newPoint)
{
   points_.push_back(newPoint);
}

void tObject::rotateRight()
{
   //generate a matrix of this object
   vector<vector<string> > matrix = getObject();

   //the resulting matrix
   vector<vector<string> > newMatrix;

   //create an empty newMatrix
   vector<string> singleRow;
   for (int x = 0; x < OBJECT_WIDTH; x++)
   {
      singleRow.clear();
      for (int y = 0; y < OBJECT_HEIGHT; y++)
      {
         singleRow.push_back(tBoard::UNOCCUPIED);
      }
      newMatrix.push_back(singleRow);
   }

   //fill array b, turn array a 90 degrees
   //b[4-c][r]=a[r][c];

   //fill array b, turn array a 180 degrees
   //b[4-r][4-c]=a[r][c];

   //fill array b, turn array a 270 degrees
   //b[c][4-r]=a[r][c];

   //rotate and save the old matrix to the new matrix
   for (int x = 0; x < OBJECT_WIDTH; x++)
   {
      for (int y = 0; y < OBJECT_HEIGHT; y++)
      {
         newMatrix[OBJECT_WIDTH-x-1][y] = matrix[y][x];
      }
   }

   //clear all the points (erase the object)
   points_.clear();

   /* make the points for the object again
   ....but here is something to take note of, we ALWAYS want
   the object to be in the top left corner of it's matrix
   so we need to save the smalles x value and the smallest y value
   and offset all the points in the object so there is atleat a
   0 for an x and a 0 for a y */

   int smallestX = OBJECT_WIDTH+5;
   int smallestY = OBJECT_WIDTH+5;
   for (int x = 0; x < OBJECT_WIDTH; x++)
   {
      for (int y = 0; y < OBJECT_HEIGHT; y++)
      {
         if (newMatrix[x][y] != tBoard::UNOCCUPIED)
         {
            points_.push_back(tPoint(x,y));
            if (x < smallestX)
               smallestX = x;
            if (y < smallestY)
               smallestY = y;
         }
      }
   }

   if (smallestX != 0)
   {
      for (int x = 0; x < points_.size(); x++)
      {
         points_[x].x_ = points_[x].x_ - smallestX;
      }
   }

   if (smallestY != 0)
   {
      for (int x = 0; x < points_.size(); x++)
      {
         points_[x].y_ = points_[x].y_ - smallestY;
      }
   }



}

vector<vector<string> > tObject::getObject()
{
   vector<vector<string> > matrix;  //create default matrix

   vector<string> singleRow;
   //set matrix to all unoccupied
   for (int x = 0; x < OBJECT_WIDTH; x++)
   {
      singleRow.clear();
      for (int y = 0; y < OBJECT_HEIGHT; y++)
      {
         singleRow.push_back(tBoard::UNOCCUPIED);
      }
      matrix.push_back(singleRow);
   }

   //now loop though each point in this object
   for (int p = 0; p < points_.size(); p++)
   {
      //char strId = id;

      matrix[points_[p].x_][points_[p].y_] = itos(id);
      //cout << "====" << matrix[points_[p].x_][points_[p].y_] << endl;
   }

   //cout << "getting object with id " << id << endl;

   return matrix;
}
