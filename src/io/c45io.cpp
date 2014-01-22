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

#include "c45io.h"
#include <stdlib.h>

const string c45io::EXE_C45 = "c4.5";
const string c45io::EXE_CONSULT = "consult";
const string c45io::FILE_DATA_EXT = ".data";
const string c45io::FILE_NAME_EXT = ".names";

c45io::c45io()
{}

c45io::~c45io()
{}

//simply add a line to the data file IF it doesn't alreday exist!
void c45io::addDataLine(string file, string data)
{
	//just append the data to the file
	system(("echo '" + data + "' >> " + file + FILE_DATA_EXT).c_str());
}

void c45io::grabDecision(string file, vector<int> attributes, int& orientation, int& horizontal)
{

	system((EXE_C45 + " -f " + file).c_str());

	//create a file with all the input required for asking c4.5 what to do
	string tempFile = "tempFileForRunningC4.5";

	//open the file and read it in
	ofstream out(tempFile.c_str());

	if (!out)
	{
		cout << " Could not open temporary output file!" << endl;
		return;
	}


	for (int x = 0; x < attributes.size();x++)
		out << attributes[x] << endl;

	out.close();

	system((EXE_CONSULT + " < " + tempFile + " > consultResults.txt" ).c_str());

	//delete the file
	system(("rm -rf " + tempFile).c_str());
}


