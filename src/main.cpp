/*
AITetris (http://www.digitalpeer.com/projects/aitetris/)
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

/** \file  The main AITetris 1.0 file.

This is the main file for aitetris.  It's responsible for operating the tetris portion,
the "driver" portion, evaluating results, ip, and everything else.
*/

#include "io/c45io.h"
#include "io/c50io.h"
#include "tetris/tgame.h"
#include "util/util.h"

#include <cstdlib>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

/** Print out program command line arguments */
void printUsage()
{
   cerr << "AITetris 1.0 by Josh Henderson (www.digitalpeer.com)" << endl;
   cerr << "Usage: aitetris [INPUTFILE] [c,w]" << endl << endl;

   cerr << "AITetris 1.0, Copyright (C) 2002 Joshua Henderson" << endl;
   cerr << "AITetris 1.0 comes with ABSOLUTELY NO WARRANTY.  This" << endl;
   cerr << "is free software, and you are welcome to redistribute" << endl;
   cerr << "it under GPL conditions." << endl << endl;
}

/** Helper function used to execute moves to the game */
void executeMoves(tGame& game,int horizontal, int orientation)
{
   //move the object into the right orientation
   //cout << "Playing orientation:" << orientation << endl;
   for (int x = 0; x < orientation; x++)
      game.changeRight();

   //move the object into the right horizontal position
   //cout << "Playing horizontal:" << horizontal << endl;
   for (int x = 0; x < horizontal; x++)
      game.moveRight();
}

/** Generate a string of data for c5.0 data file */
string makeDateLine(vector<int> attributes, int orientation, int horizontal)
{
   string answer;

   for (int x = 0; x < attributes.size(); x++)
   {
      answer = answer + itos(attributes[x]) + ",";
   }

   return answer = answer + itos(orientation) + itos(horizontal) + ".";
}

/** Count up all the "bad blocks" on the board */
int critic(vector<vector<string> > matrix)
{
   int answer=0;
   /** for each column: Search for a top block, then count
   the number of non blocks below it */
   for (int x = 0; x < tBoard::BOARD_WIDTH; x++)
   {
      for (int y = 0; y < tBoard::BOARD_HEIGHT; y++)
      {
         if (matrix[x][y] != tBoard::UNOCCUPIED)
         {
            y++;
            while (y < tBoard::BOARD_HEIGHT)
            {
               if (matrix[x][y] == tBoard::UNOCCUPIED)
                  answer++;

               y++;
            }
            break;
         }
      }
   }
   return answer;
}

/** Take a board matrix and generate a distribution of the board by
   column.  Also, take and do an "inverted" curve like you would do on
   a grading scale and make sure there is always a 0 value.
*/
vector<int> generateDistribution(vector<vector<string> > matrix, bool normalize = true)
{
   vector<int> answer;

   /* loop through each column, when you hit something that's not a 0
   then subtract that number from board height and you have your answer */
   int x;
   for (x = 0; x < tBoard::BOARD_WIDTH; x++)
   {
      for (int y = 0; y < tBoard::BOARD_HEIGHT; y++)
      {
         if (matrix[x][y] != tBoard::UNOCCUPIED)
         {
            answer.push_back(tBoard::BOARD_HEIGHT-y);
            break;
         }
      }

      //if we never ran into something, just add a zero
      if (answer.size()-1 != x)
      {
         answer.push_back(0);
      }
   }

   //make sure the smallest value is a 0
   int smallest = answer[getIndexOfSmallest(answer)];

   if (smallest != 0 && normalize)
   {
      for (int x = 0; x < answer.size(); x++)
      {
         answer[x] = answer[x] - smallest;
      }
   }

   return answer;
}


/**  This is the interface function that will find the best possible move of a list of moves
@return The best answer
*/
void findBestCase(tGame game,
                  c50io tree,
                  vector<string> possibleMoves,
                  int allowedBadBlocks,
                  int allowedLevelDiff,
                  vector<int> distribution,
                  int& horizontal, int& orientation)
{
   //ask the decision tree for an answer
   tree.grabDecision(tBoard::NAMES[game.getCurrentObjectId()],
                     distribution,
                     orientation,
                     horizontal);

   int currentObjectId = game.getCurrentObjectId();

   //get the lowest level on the board
   vector<int> tempDistribution = generateDistribution(game.getBoardMatrix(currentObjectId),false);
   sort(tempDistribution.begin(),tempDistribution.end());
   int lowestLevel = tempDistribution[0];

   //get number of bad blocks
   int badBlocksBeforeMove = critic(game.getBoardMatrix(currentObjectId));

   //execute the moves to the board
   executeMoves(game,horizontal,orientation);

   //run the game
   game.runGame();

   //get level difference
   int lowestLevelAfterMove = game.getLowestBlock(currentObjectId);

   //get the bad blocks after move
   int badBlocksAfterMove = critic(game.getBoardMatrix());

   cout << "With decision tree answer (" << orientation << "-" << horizontal << ") results are:" << endl;
   cout << "  Critic 1: Bad blocks diff on board is " << badBlocksAfterMove-badBlocksBeforeMove << "  (" << badBlocksBeforeMove << "," << badBlocksAfterMove << ")" << endl;
   cout << "  Critic 2: Level diff on move is " << lowestLevelAfterMove-lowestLevel <<"  (" <<  lowestLevel << "," <<  lowestLevelAfterMove << ")" << endl;

   /*this is an array that holds the total "critic" value for each
   possible move parallel to vector<string> possibleMoves */
   vector<int> moveScale;

   //this is just a temporary game to use so we don't screw up our original game
   tGame tempGame;

   //if the answer is acceptable, then leave it alone
   if ((lowestLevelAfterMove-lowestLevel) <= allowedLevelDiff && (badBlocksAfterMove-badBlocksBeforeMove) <= allowedBadBlocks)
   {
      cout << "*************Decision tree came up with an acceptable answer.**************" << endl;
   }
   else
   {
      cout << "Decision tree came up with a crappy answer!  Resorting to brute force!" << endl;

      //loop over all the possible answers and save the critic values to the array
      for (int x = 0; x < possibleMoves.size(); x++)
      {
         //load the game state file
         if (!(tempGame.loadGame("board.dat")))
         {
            cout << "Could not open input file: " << "board.dat" << "!" << endl;
            return;
         }

         char a[2];
         char b[2];
         strncpy(a,&possibleMoves[x][0],1);
         strncpy(b,&possibleMoves[x][1],1);

         //execute the moves to the board
         executeMoves(tempGame,atoi(b), atoi(a));

         //save the game state back to the file and settle the board
         if (!(tempGame.runGame()))
         {
            cout << "Major error running game!" << endl;
            return;
         }

         int levelOffset = tempGame.getLowestBlock(currentObjectId);
         -lowestLevel;
         int badBlocksOffset = critic(tempGame.getBoardMatrix())-badBlocksBeforeMove;

         //save the critic scale to our array for this possibleMove
         /* Here I am giving the level offset a 2x weight */
         moveScale.push_back(badBlocksOffset + (levelOffset * 2));

      }

      //analyze the critic levels and return the best one
      char a[2];
      char b[2];
      strncpy(a,&possibleMoves[getIndexOfSmallest(moveScale)][0],1);
      strncpy(b,&possibleMoves[getIndexOfSmallest(moveScale)][1],1);

      orientation = (int)atoi(a);
      horizontal = (int)atoi(b);

      //cout << "scale size is " << moveScale.size() << " and possible size is " << possibleMoves.size() << endl;
   }

   /* Now, lets get results from the move we are actually going to make */

   //load the game state file
   if (!(tempGame.loadGame("board.dat")))
   {
      cout << "Could not open input file: " << "board.dat" << "!" << endl;
      return;
   }

   //execute the moves to the board
   executeMoves(tempGame,horizontal, orientation);

   //save the game state back to the file and settle the board
   if (!(tempGame.runGame()))
   {
      cout << "Major error running game!" << endl;
      return;
   }

   lowestLevelAfterMove = tempGame.getLowestBlock(currentObjectId);
   badBlocksAfterMove = critic(tempGame.getBoardMatrix());

   cout << "Actual move (" << orientation << "-" << horizontal << ") results are:" << endl;
   cout << "  Critic 1: Bad blocks diff on board is " << badBlocksAfterMove-badBlocksBeforeMove << "  (" << badBlocksBeforeMove << "," << badBlocksAfterMove << ")" << endl;
   cout << "  Critic 2: Level diff on move is " << lowestLevelAfterMove-lowestLevel <<"  (" <<  lowestLevel << "," <<  lowestLevelAfterMove << ")" << endl;


}

/** Play and use the critic function to evaluate and save results */
void criticMode(string inputFile)
{
   cout << "Entering critic/auto mode....(ctrl c to quit)" << endl << endl;

   vector<string> possibleMoves;
   possibleMoves.push_back("00");
   possibleMoves.push_back("01");
   possibleMoves.push_back("02");
   possibleMoves.push_back("03");
   possibleMoves.push_back("04");
   possibleMoves.push_back("05");
   possibleMoves.push_back("06");
   possibleMoves.push_back("07");
   possibleMoves.push_back("08");
   possibleMoves.push_back("09");
   possibleMoves.push_back("10");
   possibleMoves.push_back("11");
   possibleMoves.push_back("12");
   possibleMoves.push_back("13");
   possibleMoves.push_back("14");
   possibleMoves.push_back("15");
   possibleMoves.push_back("16");
   possibleMoves.push_back("17");
   possibleMoves.push_back("18");
   possibleMoves.push_back("19");
   possibleMoves.push_back("20");
   possibleMoves.push_back("21");
   possibleMoves.push_back("22");
   possibleMoves.push_back("23");
   possibleMoves.push_back("24");
   possibleMoves.push_back("25");
   possibleMoves.push_back("26");
   possibleMoves.push_back("27");
   possibleMoves.push_back("28");
   possibleMoves.push_back("29");
   possibleMoves.push_back("30");
   possibleMoves.push_back("31");
   possibleMoves.push_back("32");
   possibleMoves.push_back("33");
   possibleMoves.push_back("34");
   possibleMoves.push_back("35");
   possibleMoves.push_back("36");
   possibleMoves.push_back("37");
   possibleMoves.push_back("38");
   possibleMoves.push_back("39");

   //create an instance of the tetris game
   tGame game;

   //create an abstract vector of decision trees
   //vector<c45io> trees(10);
   vector<c50io> trees(10);

   //input holders
   int orientation = 0;
   int horizontal = 0;

   int allowedSpaces = 0;
   int allowedLevelDiff = 0;

   cout << "Enter critic allowed spaces number (usually 0-5):";
   cin >> allowedSpaces;

   cout << "Enter critic2 allowed level difference (usually 0-5):";
   cin >> allowedLevelDiff;

   system(("echo 'Allowed spaces number: " + itos(allowedSpaces) + "' >> stats.tmp").c_str());
   system(("echo 'Aallowed level difference: " + itos(allowedLevelDiff) + "' >> stats.tmp").c_str());
   system("echo '' >> stats.tmp");
   system("echo '' >> stats.tmp");

   /**
   -Here's how it all plays out:
      -# load the board and the next piece
      -# find the best possible move
      -# make the move
      -# save the board
   */
   vector<int> distribution;
   while (true)
   {
      //load the game state file
      if (!(game.loadGame(inputFile,true)))
      {
         cout << "Could not open input file: " << inputFile << "!" << endl;
         return;
      }

      //reset distibution
      distribution.clear();

      //generate the pre move distribution
      distribution = generateDistribution(game.getBoardMatrix(game.getCurrentObjectId()));

      //find the best possible move
      findBestCase(game,
                   trees[game.getCurrentObjectId()],
                   possibleMoves,
                   allowedSpaces,
                   allowedLevelDiff,
                   distribution,
                   horizontal, orientation);

      cout << "Executing move: " << orientation << "-" << horizontal << endl << endl;

      //execute the moves we decided on
      executeMoves(game,horizontal,orientation);

      //run the game
      game.runGame(false);

      // save the game state back to the file and settle the board
      game.saveGame(inputFile);

      //save the move we picked
      trees[game.getCurrentObjectId()].addDataLine(
         tBoard::NAMES[game.getCurrentObjectId()],
         makeDateLine(distribution, orientation, horizontal)
      );
   }
}

/** Learn by watching the user play */
void watchMode(string inputFile)
{
   cout << "Entering watch mode....(enter 99 for any input to quit)" << endl << endl;

   //create an instance of the tetris game
   tGame game;

   //create an abstract vector of decision trees
   //vector<c45io> trees(10);
   vector<c50io> trees(10);

   //input holders
   int orientation = 0;
   int horizontal = 0;

   /**
   -Here's how it all plays out:
      -# load the board and the next piece and display it
      -# evaluate the board
         - Create the board distribution
         - Decide in the piece we are using
      -# ask the user for input
      -# save the input to c4.5 data file
      -# run tetris with input
   */
   vector<int> distribution;
   while (true)
   {
      //load the game state file
      if (!(game.loadGame(inputFile,true)))
      {
         cout << "Could not open input file: " << inputFile << "!" << endl;
         return;
      }

      distribution.clear();

      //generate the distribution
      distribution = generateDistribution(game.getBoardMatrix(game.getCurrentObjectId()));

      //cout << "DISTRIBUTION:" << makeDateLine(distribution, -1,-1) << endl;

      //move the object into the right orientation
      cout << "Enter orientation (0-3):";
      cin >> orientation;
      if (orientation == 99)
         return;

      //move the object into the right horizontal position
      cout << "Enter horizontal alignment (0-14):";
      cin >> horizontal;
      if (horizontal == 99)
         return;

      //save the current state to the tree before we do anything
      trees[game.getCurrentObjectId()].addDataLine(
         tBoard::NAMES[game.getCurrentObjectId()],
         makeDateLine(distribution, orientation, horizontal)
      );

      cout << "Saved current state: " << makeDateLine(distribution, orientation, horizontal) << endl << endl;

      //execute the moves
      executeMoves(game,horizontal,orientation);

      //run the game
      game.runGame(false);

      // save the game state to the file
      game.saveGame(inputFile,false);

   }
}


int main(int argc,char *argv[])
{
   srand(seedGen()/*time(0)*/) ;

   //make sure we got what we wanted on the command line
   if (argc < 2)
   {
      cerr << "Not enough parameters" << endl << endl;
      printUsage();
      return 1;
   }

   //get the input/output file name
   string inputFile = argv[1];

   cout << "Reading input file " << inputFile << endl;

   string parameter = "";

   if (argc == 3)
   {
      parameter = *argv[2];
   }

   system(("echo 'Mode: " + parameter + "' >> stats.tmp").c_str());

   if (parameter == "w")
   {
      watchMode(inputFile);
   }
   else if (parameter == "c")
   {
      criticMode(inputFile);
   }
   else
   {
      cout << "No operation mode specified (w or c)!" << endl;
   }

   cout << endl << "AITetris Safely Exiting" << endl;

   return 0;
}

