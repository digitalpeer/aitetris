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

#ifndef C50_IO_H
#define C50_IO_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <stdio.h>
using namespace std;

/** This class is used for interaction with the c5.0 */
class c50io
{
	public:

		static const string EXE_C50;
		static const string EXE_CONSULT;

		//filename extensions
		static const string FILE_DATA_EXT;
		static const string FILE_NAME_EXT;
		static const string FILE_CASES_EXT;

		/** Init the consulter with the filestem */
		c50io();

		void grabDecision(string file, vector<int> attributes, int& orientation, int& horizontal);

		//simply add a line to the data file IF it doesn't alreday exist!
		void addDataLine(string file, string data);

		~c50io();
};


#endif
