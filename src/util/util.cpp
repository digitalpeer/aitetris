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

#include "util.h"

/** Convert int to string */
string itos(int i)
{
   stringstream s;
   s << i;
   return s.str();
}

/** Generate a random seed */
long seedGen()
{
   srand(time(NULL));
   long newseed = rand();
   newseed = rand();
   newseed = rand();
   newseed = newseed*newseed;

   return newseed;
}

/** Get the index of the smallest integer in an array */
int getIndexOfSmallest(vector<int> array)
{
   int answer = 0;
   int smallest = array[0];
   for (int x = 1; x < array.size(); x++)
   {
      if (array[x] < smallest)
      {
         smallest = array[x];
         answer = x;
      }
   }
   return answer;
}
