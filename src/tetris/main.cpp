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

/*
  This is the tetris program.
*/

#include "tgame.h"

#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;

void printUsage()
{
   cout << "tetris [BOARDSTATEFILE] [HORIZONTALACTION] [ORIENTATIONACTION]" << endl;
   cout << "where" << endl;
   cout << "BOARDSTATEFILE is the input file name" << endl;
   cout << "HORIZONTALACTION is a number 0 - 9" << endl;
   cout << "ORIENTATIONACTION is a number 0 - 4" << endl << endl;
}

int main(int argc,char *argv[])
{
   if (argc < 4)
   {
      cout << "Not enough parameters" << endl << endl;
      printUsage();
      return 1;
   }
   else if (!isdigit(*argv[2]) || !isdigit(*argv[3]))
   {
      cout << "Invalid parameters" << endl << endl;
      printUsage();
      return 1;
   }

   //create an instance of the game
   tGame game;

   //get the input/output file name
   string inputFile = argv[1];

   //load the game state file
   if (!(game.loadGame(inputFile.c_str())))
   {
      cout << "Could not open input file: " << inputFile << "!" << endl;
      return 1;
   }

   //move the object into the right orientation
   int orientation = atoi(argv[2]);
   for (int x = 0; x < orientation; x++)
      game.changeRight();

   //move the object into the right horizontal position
   int horizontal = atoi(argv[3]);
   for (int x = 0; x < horizontal; x++)
      game.moveRight();

   // save the game state
   if (!(game.saveGame(inputFile.c_str())))
   {
      cout << "Could not save the game state to file: " << inputFile << "!" << endl;
      return 1;
   }

   return 0;
}
