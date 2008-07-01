/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   This file is part of Einstein@Home.                                   *
 *                                                                         *
 *   Einstein@Home is free software: you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published     *
 *   by the Free Software Foundation, version 2 of the License.            *
 *                                                                         *
 *   Einstein@Home is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include <cstdlib>
#include <string>
#include <iostream>

#include "ResourceCompiler.h"

void printUsage() {
	cerr << "Invalid command-line options!" << endl;
	cerr << "Usage: orc <input filename> <output filename>" << endl;
}

int main(int argc, char *argv[])
{
	if(argc != 3) {
		printUsage();
		exit(1);
	}
	else {
		string inputFilename(argv[1]);
		string outputFilename(argv[2]);
		
		// TODO: better filename checking
		if( inputFilename == "." || inputFilename == ".." ||
			outputFilename == "." || outputFilename == "..")
		{
			printUsage();
			exit(1);
		}
		
		ResourceCompiler rc(inputFilename, outputFilename);
		rc.compile();
		
		exit(0);
	}
}
