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
