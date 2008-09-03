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

#ifndef LIBXML2ADAPTER_H_
#define LIBXML2ADAPTER_H_

#include <string>

#include "XMLProcessorInterface.h"

#include <libxml/parser.h>

using namespace std;

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief This class implements XMLProcessorInterface providing an adapter to libxml2
 *
 * For the time being libxml2 is considered to be the only interesting XML processing
 * library because of its rich feature set that comprises DOM, SAX, XPath, XSLT as well
 * as validation based on DTD and XML Schema.
 *
 * \see XMLProcessorInterface
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class Libxml2Adapter : public XMLProcessorInterface
{
public:
	/// Constructor
	Libxml2Adapter();

	/// Destructor
	virtual ~Libxml2Adapter();

	/**
	 * \brief Sets the XML document to be processed
	 *
	 * \param xml The XML document to be used for processing
	 * \param url The base URL of the document
	 */
	void setXmlDocument(const string xml, const string url);

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
	string getSingleNodeContentByXPath(const string xpath);

	/**
	 * \brief Retrieves a single node's (element or attribute) content
	 *
	 * This method takes a XPath expression that properly defines how to search
	 * for a specific node (element or attribute). If the XPath search results
	 * in more than one instance or if the node (or its content) can't be found
	 * the return value is an empty string.
	 *
	 * \param xml The XML document to be used for processing
	 * \param url The base URL of the document
	 * \param xpath The XPath expression to be used for seaching
	 *
	 * \return The node's content or an empty string in case of an error
	 */
	string getSingleNodeContentByXPath(const string xml, const string url, const string xpath);

private:
	/// The current XML document instance
	xmlDocPtr m_xmlDocument;
};

/**
 * @}
 */

#endif /*LIBXML2ADAPTER_H_*/
