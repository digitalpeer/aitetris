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


#include "defns.h"
#include "global.cpp"
#include "hooks.cpp"

#include "c50io.h"

const string c50io::EXE_C50 = "c5.0";
const string c50io::EXE_CONSULT = "consult";
const string c50io::FILE_DATA_EXT = ".data";
const string c50io::FILE_NAME_EXT = ".names";
const string c50io::FILE_CASES_EXT = ".cases";


/** Init the consulter with the filestem */
c50io::c50io()
{}


//simply add a line to the data file IF it doesn't alreday exist!
void c50io::addDataLine(string file, string data)
{
	//just append the data to the file
	system(("echo '" + data + "' >> " + file + FILE_DATA_EXT).c_str());
}

void c50io::grabDecision(string file, vector<int> attributes, int& orientation, int& horizontal)
{
	char temp[25];
	strcpy(temp,file.c_str());
	FileStem = temp;


	//take and generate the tree
	system((EXE_C50 + " -f " + file + " > treeresults.tmp").c_str());


	//generate the case file
	//cout << "generating file " << file <<  FILE_CASES_EXT << endl;
	//open the file and read it in
	ofstream out((file + FILE_CASES_EXT).c_str());

	if (!out)
	{
		cout << " Could not open temporary output file!" << endl;
		return;
	}


	for (int x = 0; x < attributes.size();x++)
	{
		out << attributes[x];

		if (x == attributes.size()-1)
			out << ",?." << endl << endl;
		else
			out  << ",";
	}


	out.close();


	//cout << "Working with file results: " << file << endl;


	FILE		*F;
	Description	Case;
	int		CaseNo=0, MaxClassLen=5, o;
	ClassNo		Predict, c;
	Boolean		XRefForm=false;
	extern String	OptArg, Option;

	/*  Read information on attribute names, values, and classes  */
	if ( ! (F = GetFile(".names", "r")) )
		Error(NOFILE, Fn, "");

	GetNames(F);

	/*  Read the appropriate classifier file.  Call CheckFile() to
	determine the number of trials, then allocate space for
	trees or rulesets  */

	CheckFile(".tree", false);
	Pruned = AllocZero(TRIALS+1, Tree);

	ForEach(Trial, 0, TRIALS-1)
	{
	Pruned[Trial] = GetTree(".tree");
	}


	/*  Close the classifier file and reset the file variable  */

	fclose(TRf);
	TRf = 0;

	/*  Now classify the cases in file <filestem>.cases.
	This has the same format as a .data file except that
	the class can be "?" to indicate that it is unknown.  */

	//printf("Case\t\tGiven\t\tPredicted\n %s\t\tClass\t\tClass\n\n",
	//	( LabelAtt ? "ID" : "No" ));


	if ( ! (F = GetFile(".cases", "r")) )
		Error(NOFILE, Fn, "");

	ClassSum = AllocZero(MaxClass+1, float);  /* used in classification */
	Vote     = AllocZero(MaxClass+1, float);  /* used with boosting */

	LineNo = 1;

	while ( (Case = GetDescription(F, false)) )
	{
		Predict = Classify(Case);

		/*  Print either case label or number  */

		//if ( LabelAtt )
		//{
		//printf("%-15.15s ", SVal(Case,LabelAtt));
		//}
		//else
		//{
		//printf("%4d\t\t", ++CaseNo);
		//}

		//printf("%-15.15s %-15.15s [%.2f]\n",
		//	ClassName[Class(Case)], ClassName[Predict], Confidence);
		//char o = ClassName[Predict][0];
		//orientation = atoi(&o);
		//horizontal = atoi(&ClassName[Predict][1]);

		char a[2];
		char b[2];
		strncpy(a,&ClassName[Predict][0],1);
		strncpy(b,&ClassName[Predict][1],1);

		//analyze the critic levels and return the best one
		orientation = (int)atoi(a);
		horizontal = (int)atoi(b);

		FreeLastCase(Case);
	}

	/*  Close the case file and free allocated memory  */
	fclose(F);
	FreeGlobals();
}

c50io::~c50io()
{}
