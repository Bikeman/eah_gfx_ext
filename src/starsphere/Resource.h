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

#ifndef RESOURCE_H_
#define RESOURCE_H_

#include <string>
#include <vector>

using namespace std;

/**
 * \brief Class that serves as storage container for arbitrary binary data resources
 * 
 * Instances of this class are returned by ResourceFactory and hold any kind of binary data
 * in the guise of a byte value vector identified by a descriptive string. You can access the
 * data using \ref data() and query the identifier using \ref identifier(). 
 * 
 * \see ResourceFactory
 * 
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class Resource
{
public:
	/**
	 * \brief Constructor
	 * 
	 * \param identifier The string used to identify this resource
	 * \param data A vector of byte values used to initialize the resource's data container
	 */
	Resource(const string identifier, const vector<unsigned char>& data);
	
	/// Destructor
	virtual ~Resource();
	
	/**
	 * \brief Retrieve the identifier of this resource
	 * 
	 * \return The identifier of this resource
	 */
	string identifier() const;
	
	/**
	 * \brief Retrieve the data stored in this resource
	 * 
	 * The data can be accessed using the constant pointer returned by this method.
	 * The pointer points to the beginning of a byte value sequence. The length of the
	 * vector equals the the size of the data content.
	 * 
	 * \return The base pointer to the data contained in this resource
	 */
	const vector<unsigned char> * data() const;
	
private:
	/// The identifer of this resource
	string m_Identifier;
	
	/// The data storage container of this resource
	const vector<unsigned char> m_Data;
};

#endif /*RESOURCE_H_*/
