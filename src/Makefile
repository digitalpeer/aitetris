#
#AITetris (http://www.digitalpeer.com/projects/aitetris/)
#Copyright (C) 2002  Josh Henderson (digitalpeer)
#
#This program is free software; you can redistribute it and/or
#modify it under the terms of the GNU General Public License
#as published by the Free Software Foundation; either version 2
#of the License, or (at your option) any later version.

#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.

#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#

# source files
SRC = 	main.cpp \
	tetris/tboard.cpp tetris/tgame.cpp tetris/tobject.cpp tetris/tpoint.cpp \
	io/c45io.cpp \
	io/c50io.cpp \
	util/util.cpp

OBJ = $(SRC:.cpp=.o)

# output file
OUT = aitetris

# include directories
INCLUDES =

# flags -O2 -Wall
CFLAGS = -g -fpermissive

# And put any C++ compiler flags here.
CCFLAGS = $(CFLAGS)

# CC speficies the name of the C compiler; CCC is the C++ compiler.
CCC = g++

# libraries.
LIBS = 
LDFLAGS =

.SUFFIXES: .cpp

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) $(CCFLAGS) $(OBJ) $(LDFLAGS) $(LIBS) -o $(OUT)

clean:
	rm -f $(OBJ) aitetris 

#$(OUT)
