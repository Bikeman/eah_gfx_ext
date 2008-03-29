#ifndef RESOURCEFACTORY_H_
#define RESOURCEFACTORY_H_

#include <string>
#include <map>
#include <iostream>

#include "Resource.h"

class ResourceFactory
{
public:
	ResourceFactory();
	virtual ~ResourceFactory();
	
	const Resource* createInstance(const string identifier);
	
private:
	map<string, vector<unsigned char> > m_ResourceMap;
};

// TODO: does this need to be global?
extern const string c_ResourceIdentifiers[];
extern const unsigned int c_ResourceIndex[][2];
extern const unsigned char c_ResourceStorage[];

#endif /*RESOURCEFACTORY_H_*/
