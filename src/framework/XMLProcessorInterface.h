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

#ifndef XMLPROCESSORINTERFACE_H_
#define XMLPROCESSORINTERFACE_H_

#include <string>

using namespace std;

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief This interface declares all mandatory public methods to be provied by any
 * given XML Processor.
 *
 * All classes implementing this interface have to provide at least the set of methods
 * declared here.
 *
 * \see Libxml2Adapter
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class XMLProcessorInterface
{
public:

	/// Empty Destructor
	virtual ~XMLProcessorInterface() {};

	/**
	 * \brief Sets the XML document to be processed
	 *
	 * \param xml The XML document to be used for processing
	 * \param url The base URL of the document
	 */
	virtual void setXmlDocument(const string xml, const string url) = 0;

	/**
	 * \brief Retrieves a single node's (element or attribute) content
	 *
	 * This method takes a XPath expression that properly defines how to search
	 * for a specific node (element or attribute). If the XPath search results
	 * in more than one instance or if the node (or its content) can't be found
	 * the return value is NULL.
	 *
	 * \param xpath The XPath expression to be used for seaching
	 *
	 * \return The node's content or NULL in case of an error
	 */
	virtual string getSingleNodeContentByXPath(const string xpath) = 0;

	/**
	 * \brief Retrieves a single node's (element or attribute) content
	 *
	 * This method takes a XPath expression that properly defines how to search
	 * for a specific node (element or attribute). If the XPath search results
	 * in more than one instance or if the node (or its content) can't be found
	 * the return value is NULL.
	 *
	 * \param xml The XML document to be used for processing
	 * \param url The base URL of the document
	 * \param xpath The XPath expression to be used for seaching
	 *
	 * \return The node's content or NULL in case of an error
	 */
	virtual string getSingleNodeContentByXPath(const string xml, const string url, const string xpath) = 0;
};

/**
 * @}
 */

#endif /*XMLPROCESSORINTERFACE_H_*/
