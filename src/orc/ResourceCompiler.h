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

#ifndef RESOURCECOMPILER_H_
#define RESOURCECOMPILER_H_

#include <cstdlib>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

/**
 * \brief Helper class that converts binary resources into source code ready for compilation
 * 
 * This "compiler" takes a resource specification file, opens and loads the physical
 * files and converts their contents into normal C/C++ source code. The source code
 * comprises three arrays which can be subsequently compiled into object code which is
 * then referenced by ResourceFactory using external linkage.
 * \n
 * %Resource specification file format:
 * - Each line describes one resource
 * - The descriptor has to look like this: LogicalResourceName|PhysicalResourceName
 * - Lines starting with # are treated as comments
 * - Empty lines are ignored
 * 
 * As you can see the logical and the physical (file) resource name are delimited by the pipe (|) character.
 * The logical resource name will be used again later. It is the identifier used to request a resource via
 * ResourceFactory::createInstance() 
 * 
 * \see ResourceFactory
 * \see ResourceFactory::c_ResourceIdentifiers
 * \see ResourceFactory::c_ResourceIndex
 * \see ResourceFactory::c_ResourceStorage
 * 
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class ResourceCompiler
{
public:
	/**
	 * \brief Constructor
	 * 
	 * \param inputFilename Name of the resource specification file (source, \c *.orc)
	 * \param outputFilename Name of the converted recources file (destination, \c *.cpp)
	 */
	ResourceCompiler(const string inputFilename, const string outputFilename);
	
	/// Destructor
	virtual ~ResourceCompiler();
	
	/**
	 * \brief Converts the specified resources into the specified source code file
	 * 
	 * It iterates over all resources found in the local cache and stores their data
	 * and meta information as source code in the destination file. Thus parseInputFile()
	 * and loadBinaryData() have to be called first for this to work.
	 * 
	 * \see parseInputFile
	 * \see loadBinaryData
	 */
	void compile();
	
private:
	/**
	 * \brief Parses the specified input file
	 * 
	 * After validating the resource specification file its contents are
	 * stored for later use.
	 * 
	 * \see loadBinaryData
	 */
	void parseInputFile();
	
	/**
	 * \brief Loads binary resource file data into the local cache
	 * 
	 * This methods tries to open all resource files found by loadBinaryData()
	 * and copies their binary data into the local cache.
	 * 
	 * \see parseInputFile
	 */
	void loadBinaryData();
	
	/// Path and filename of the resource specification file (source)
	string m_ResourceSpecFile;
	
	/// Path and filename of the converted source code file (destination)
	string m_ResourceCodeFile;
	
	/// Mapping between logical and physical resource names
	map<string, string> m_ResourceFileMap;
	
	/// %Resource cache (identified by logical resource name)
	map<string, vector<unsigned char> > m_ResourceDataMap;
};

#endif /*RESOURCECOMPILER_H_*/
