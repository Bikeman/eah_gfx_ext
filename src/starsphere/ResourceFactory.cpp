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

#include "ResourceFactory.h"

ResourceFactory::ResourceFactory()
{
	// determine number of resources
	int resourceCount = c_ResourceIndex[0][0];
	
	// import each resource into factory cache
	for(int i = 0; i < resourceCount; ++i) {
		
		// prepare temporary buffer
		size_t resourceSize = c_ResourceIndex[i+1][1];
		vector<unsigned char> buffer(resourceSize);

		// extract resource data from storage container
		for(size_t x = 0; x < resourceSize; ++x) {
			
			// use offset and relative position to find the absolute position
			unsigned char byteValue = c_ResourceStorage[c_ResourceIndex[i+1][0] + x];
			
			// add byte to buffer
			buffer[x] = byteValue;
		}
		
		// add buffer to resource map
		m_ResourceMap[c_ResourceIdentifiers[i]] = buffer;
	}
}

ResourceFactory::~ResourceFactory()
{
}

const Resource* ResourceFactory::createInstance(const string identifier)
{
	Resource *res = NULL;
	
	// determine whether the requested identifier exists
	if(m_ResourceMap.find(identifier) != m_ResourceMap.end()) {
		// we know the requested resource, create instance
		res = new Resource(identifier, m_ResourceMap[identifier]);
	}
	
	return res;
}
