#ifndef RESOURCECOMPILER_H_
#define RESOURCECOMPILER_H_

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;


class ResourceCompiler
{
public:
	ResourceCompiler(const string inputFilename, const string outputFilename);
	virtual ~ResourceCompiler();
	
	void compile();
	
private:
	void parseInputFile();
	void loadBinaryData();
	
	string m_ResourceSpecFile;
	string m_ResourceCodeFile;
	map<string, string> m_ResourceFileMap;
	map<string, vector<unsigned char> > m_ResourceDataMap;
};

#endif /*RESOURCECOMPILER_H_*/
