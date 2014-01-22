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

#include <cstdlib>
#include <ctime>
#include <iomanip>

#include "tgame.h"
#include "../util/util.h"

tGame::tGame()
{
   //take and create the default objects (this only needs to be done one time)
   generateDefaultObjects();

   reset();
}

tGame::~tGame()
{}

void tGame::reset()
{
   //clear the board
   board_.clear();

   //init variables
   gamesPlayed = 0;
   numberOfRuns = 0;
   numberOfLines = 0;
   playable = false;

   currentObject.id = -1;
}

void tGame::generateDefaultObjects()
{

   tObject dummy;

   //add two dummy objects onto the front
   defaultObjects.push_back(dummy);
   defaultObjects.push_back(dummy);

   tObject block;
   block.id = tObject::BLOCK;
   block.addPoint(tPoint(0,0));
   block.addPoint(tPoint(1,0));
   block.addPoint(tPoint(0,1));
   block.addPoint(tPoint(1,1));
   defaultObjects.push_back(block);


   tObject lslant;
   lslant.id = tObject::LSLANT;
   lslant.addPoint(tPoint(1,0));
   lslant.addPoint(tPoint(1,1));
   lslant.addPoint(tPoint(0,1));
   lslant.addPoint(tPoint(0,2));
   defaultObjects.push_back(lslant);

   tObject rslant;
   rslant.id = tObject::RSLANT;
   rslant.addPoint(tPoint(0,0));
   rslant.addPoint(tPoint(0,1));
   rslant.addPoint(tPoint(1,1));
   rslant.addPoint(tPoint(1,2));
   defaultObjects.push_back(rslant);

   tObject stick;
   stick.id = tObject::STICK;
   stick.addPoint(tPoint(0,0));
   stick.addPoint(tPoint(0,1));
   stick.addPoint(tPoint(0,2));
   stick.addPoint(tPoint(0,3));
   defaultObjects.push_back(stick);


   tObject middle;
   middle.id = tObject::MIDDLE;
   middle.addPoint(tPoint(0,0));
   middle.addPoint(tPoint(0,1));
   middle.addPoint(tPoint(0,2));
   middle.addPoint(tPoint(1,1));
   defaultObjects.push_back(middle);
}

bool tGame::isGameOver()
{
   bool answer;

   answer = board_.doesObjectConflict(currentObject);

   return answer;
}


bool tGame::moveLeft()
{
   bool answer = false;
   if (playable)
      answer = board_.moveObject(currentObject.id,tBoard::LEFT);
   return answer;
}

bool tGame::moveRight()
{
   bool answer = false;
   if (playable)
      answer = board_.moveObject(currentObject.id,tBoard::RIGHT);
   return answer;
}

void tGame::changeLeft()
{
   if (playable)
      board_.rotateObject(currentObject.id,tBoard::LEFT);
}

void tGame::changeRight()
{
   if (playable)
      board_.rotateObject(currentObject.id,tBoard::RIGHT);
}

//settle the current object all the way to the bottom
void tGame::settleBoard()
{
   for (int y = 0; y < tBoard::BOARD_HEIGHT; y++)
      board_.moveObject(currentObject.id,tBoard::DOWN);
}


bool tGame::loadGame(string fileName, bool showBoard)
{
   //cout << "Loading game" << endl;
   string word;

   //open the file and read it in
   ifstream in(fileName.c_str());

   if (!in)
   {
      cout << " Could not open input file!" << endl;
      return false;
   }

   //load in the number of runs
   in >> word;
   numberOfRuns = atoi(word.c_str());
   if (showBoard)
      cout << "Number of runs: " << numberOfRuns << endl;

   //load in the number of lines made
   in >> word;
   numberOfLines = atoi(word.c_str());
   if (showBoard)
      cout << "Number of lines: " << numberOfLines << endl;

   //load in the number of games played
   in >> word;
   gamesPlayed = atoi(word.c_str());
   if (showBoard)
      cout << "Number of games played: " << gamesPlayed << endl;


   //load in the next object
   currentObject.clear();
   for (int y = 0; y < tObject::OBJECT_HEIGHT; y++)
   {
      for (int x = 0; x < tObject::OBJECT_WIDTH; x++)
      {
         in >> word;
         if (word != tBoard::UNOCCUPIED)
         {
            currentObject.id = atoi(word.c_str());
            currentObject.addPoint(tPoint(x,y));
            playable = true;
         }

         if (showBoard)
            cout << setw(3) << word;
      }
      if (showBoard)
         cout << endl;
   }

   if (showBoard)
      cout << endl << "Current object:" << tBoard::NAMES[currentObject.id] << "(" << currentObject.id << ")" << endl;

   if (showBoard)
   {
      cout << endl << endl;

      for (int x = 0; x < tBoard::BOARD_WIDTH; x++)
      {
         cout << setw(3) << x;
      }
      cout << endl;
   }

   //load in the board
   tObject boardState;
   for (int y= 0; y < tBoard::BOARD_HEIGHT; y++)
   {
      for (int x = 0; x < tBoard::BOARD_WIDTH; x++)
      {
         in >> word;

         if (word != tBoard::UNOCCUPIED)
         {
            //boardState.id = atoi(word.c_str());
            boardState.addPoint(tPoint(x,y));
         }

         if (showBoard)
            cout << setw(3) << word;
      }
      if (showBoard)
         cout << endl;
   }

   boardState.id = tObject::BOARD;

   //there should be nothing on the board at this point
   board_.clear();

   board_.addObject(boardState);

   if (playable)
   {
      //cout << "Loaded board state and current object id " << currentObject.id << endl;
      board_.addObject(currentObject);
   }

   if (in.fail())
   {
      cout << "There was a error parsing input file!" << endl;
   }
   //close the file
   in.close();

   return true;
}

int tGame::getCurrentObjectId()
{
   return currentObject.id;
}


vector<vector<string> > tGame::getBoardMatrix(int excludeObjectId)
{
   return board_.getBoard(excludeObjectId);
}

bool tGame::runGame(bool hideOutput)
{
   //settle the board
   settleBoard();

   //remove the lines
   numberOfLines += board_.removeLines();

   if (isGameOver())
   {
      board_.clear();
      currentObject.clear();
      gamesPlayed++;

      if (!hideOutput)
         cout << "Game over!" << endl;

      return false;
   }

   //ran another time
   ++numberOfRuns;

   //ok, we need to pick the next object to use, so pick one by random
   int newObjectID = (rand() % 5)+2;
   currentObject = defaultObjects[newObjectID];
   currentObject.id = newObjectID;

   return true;
}

void tGame::saveGame(string fileName,bool hideOutput)
{
   //open the file and read it in
   ofstream out(fileName.c_str());

   if (!out)
   {
      if (!hideOutput)
         cout << " Could not open output file!" << endl;

      return;
   }

   /** This is just some simple stats generating stuff that's not
   required for normal operation */
   if ((numberOfRuns % 100) == 0)
   {
      system(("echo 'Number of runs: " + itos(numberOfRuns) + "' >> stats.tmp").c_str());
      system(("echo 'Number of loses: " + itos(gamesPlayed) + "' >> stats.tmp").c_str());
      system(("echo 'Number of lines: " + itos(numberOfLines) + "' >> stats.tmp").c_str());
      system("echo '' >> stats.tmp");
   }

   out << setw(3) << numberOfRuns << endl << endl;
   out << setw(3) << numberOfLines << endl << endl;
   out << setw(3) << gamesPlayed << endl << endl;

   //get the board without removed lines
   vector<vector<string> > printBoard = board_.getBoard(-99);

   //save the object
   vector<vector<string> > printObject = currentObject.getObject();
   for (int y = 0; y < tObject::OBJECT_HEIGHT; y++)
   {
      for (int x = 0; x < tObject::OBJECT_WIDTH; x++)
      {
         out << setw(3) << printObject[x][y];
      }
      out << endl;
   }

   out << endl << endl;

   //save the board
   for (int y = 0; y < tBoard::BOARD_HEIGHT; y++)
   {
      for (int x = 0; x < tBoard::BOARD_WIDTH; x++)
      {
         out << setw(3) << printBoard[x][y];
      }
      out << endl;
   }

   //close the file
   out.close();
}

int tGame::getLowestBlock(int objectId)
{
   int answer = 0;
   tObject current = board_.getObject(objectId);

   /* simply loop through all the points in the object and find the one with the
   highest y value */
   for (int x = 0; x < current.points_.size(); x++)
   {
      if (current.points_[x].y_ > answer)
         answer = current.points_[x].y_;
   }

   //take and inverse the answer so we are starting from the bottom
   answer = tBoard::BOARD_HEIGHT - answer;

   return answer;
}
