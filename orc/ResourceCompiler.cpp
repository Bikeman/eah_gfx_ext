#include "ResourceCompiler.h"

ResourceCompiler::ResourceCompiler(const string inputFilename, const string outputFilename)
{
	m_ResourceSpecFile = inputFilename;
	m_ResourceCodeFile = outputFilename;
}

ResourceCompiler::~ResourceCompiler()
{
}

void ResourceCompiler::compile()
{
	// parse input file (resource <-> file mapping)
	parseInputFile();
	
	// load the binary files (resource <-> data mapping)
	loadBinaryData();
	
	// temporary variables
	ostringstream resourceIdentifierInitializer;
	ostringstream resourceIndexInitializer;
	ostringstream resourceStorageInitializer;
	
	map<string, vector<unsigned char> >::iterator mapPos;
	vector<unsigned char>::iterator dataPos;
	unsigned int currentIndex = 0;
	
	// store total amount of resources
	resourceIndexInitializer << "{0x" << hex << m_ResourceDataMap.size() << ", 0x0},";
	
	// iterate over all resource data mappings we have
	for(mapPos = m_ResourceDataMap.begin(); mapPos != m_ResourceDataMap.end(); ++mapPos) {
		
		// store identifier
		resourceIdentifierInitializer << "\"" << mapPos->first << "\",";
		
		// store data base index
		resourceIndexInitializer << "{0x" << hex << currentIndex << ",";
		resourceIndexInitializer << "0x" << hex << mapPos->second.size() << "},";
		currentIndex += mapPos->second.size();

		// iterate over the data content byte by byte
		for(dataPos = mapPos->second.begin(); dataPos != mapPos->second.end(); ++dataPos) {
			// store byte value as part of array initializer
			resourceStorageInitializer << "0x" << hex << (int)*dataPos << ",";
		}
	}
	
	// open the output code file
	ofstream outputFile(m_ResourceCodeFile.c_str(), ios::out);
	if(!outputFile) {
		cerr << "Couldn't open output file \"" <<  m_ResourceCodeFile << "\"!" << endl,		
		exit(1);
	}
	
	// let's get some exceptions
	outputFile.exceptions(ios::failbit | ios::badbit);
	
	try {
		// write header
		outputFile << "#include <string>" << endl << endl;
		
		// write code file contents (remove trailing commas)
		string output = resourceIdentifierInitializer.str();
		outputFile << "extern const std::string c_ResourceIdentifiers[] = {" << endl;
		outputFile << output.substr(0, output.length() - 1);
		outputFile << endl << "};" << endl << endl;
		
		output = resourceIndexInitializer.str();
		outputFile << "extern const unsigned int c_ResourceIndex[][2] = {" << endl;		
		outputFile << output.substr(0, output.length() - 1);
		outputFile << endl << "};" << endl << endl;
		
		output = resourceStorageInitializer.str();
		outputFile << "extern const unsigned char c_ResourceStorage[] = {" << endl;
		outputFile << output.substr(0, output.length() - 1);
		outputFile << endl << "};" << endl << endl;
	}
	catch(const ios::failure& error) {
		cerr << "Error during output file processing: " << error.what() << endl;
	}
	
	// clean up and clode file
	outputFile.flush();
	outputFile.close();
}

void ResourceCompiler::parseInputFile()
{
	// open input file
	ifstream inputFile(m_ResourceSpecFile.c_str(), ios::in);
	if(!inputFile) {
		cerr << "Couldn't open input file \"" <<  m_ResourceSpecFile << "\"!" << endl,
		exit(1);
	}
	
	// let's get some exceptions
	inputFile.exceptions(ios::failbit | ios::badbit);
	
	try {
		string line;
		// read input file line by line
		while(getline(inputFile, line)) {
			
			unsigned int firstCharacter = line.find_first_not_of(" \t\r\n\f");
			
			// we (sort of) allow for empty lines and comments
			if(firstCharacter != string::npos && line.substr(firstCharacter, 1) != "#") { 
				
				// find our token delimiter
				unsigned int separator = line.find("|");
				
				// make sure there's exactly one delimiter
				if(separator == string::npos || separator != line.rfind("|")) {
					cerr << "Unexpected resource specification: " << line << endl;
				}
				else {
					// store tokens in resource file map
					m_ResourceFileMap[line.substr(0, separator)] = line.substr(separator + 1);
				}
			}
		}
	}
	catch(const ios::failure& error) {
		// check stream state for real error
		if(!inputFile.eof()) {
			cerr << "Error during input file processing: " << error.what();
		}		
	}
	
	// close input file
	inputFile.close();
}

void ResourceCompiler::loadBinaryData()
{
	map<string, string>::iterator pos;
	
	// iterate over all resource file mappings we have
	for(pos = m_ResourceFileMap.begin(); pos != m_ResourceFileMap.end(); ++pos) {
		
		// open given resource file
		ifstream binaryFile(pos->second.c_str(), ios::in | ios::binary);
		if(!binaryFile) {
			cerr << "Couldn't open binary file \"" <<  pos->second << "\"!" << endl,
			exit(1);
		}
		
		// let's get some exceptions
		binaryFile.exceptions(ios::failbit | ios::badbit);
		
		try {
			// store binary resource file using stream iterators
//			copy(	istream_iterator<unsigned char>(binaryFile),
//					istream_iterator<unsigned char>(),
//					back_inserter(m_ResourceDataMap[pos->first])
//			);
			
			// "copy" ends prematurely, so use the crude way instead
			char c = 0;
			while(binaryFile.get(c)) {
				m_ResourceDataMap[pos->first].push_back(c);
			}
		}
		catch(const ios::failure& error) {
			// check stream state for real error
			if(!binaryFile.eof()) {
				cerr << "Error during binary file processing: " << error.what();
			}	
		}

		// close current file
		binaryFile.close();
	}
}
