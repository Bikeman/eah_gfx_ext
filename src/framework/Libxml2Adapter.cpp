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

#include "Libxml2Adapter.h"

#include <iostream>
#include <sstream>

#include <libxml/xpath.h>

Libxml2Adapter::Libxml2Adapter()
{
	m_xmlDocument = NULL;
}

Libxml2Adapter::~Libxml2Adapter()
{
}

void Libxml2Adapter::setXmlDocument(const string xml, const string url)
{
	xmlDocPtr doc = xmlReadMemory(xml.c_str(), xml.size(), url.c_str(), NULL, 0);
	if(doc) {
		m_xmlDocument = doc;
	}
	else {
		cerr << "Document couldn't be parsed!" << endl;
	}
}

string Libxml2Adapter::getSingleNodeContentByXPath(const string xpath)
{
	// no document available!
	if(!m_xmlDocument) return("");

	// prepare xpath search
	stringstream buffer;
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(m_xmlDocument);
    xmlChar* xpathExpr = xmlCharStrdup(xpath.c_str());

    // run xpath query
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    // how many nodes did we find?
    int size = (nodes) ? nodes->nodeNr : 0;

    if(size == 0) {
    	cerr << "No node found using XPath expression: " << xpath << endl;
    	return("");
    }
    else if(size < 1) {
    	cerr << "More than node found using XPath expression: " << xpath << endl;
    	return("");
    }

    // convert xml contents
    buffer << xmlNodeListGetString(m_xmlDocument, nodes->nodeTab[0]->xmlChildrenNode, 1);

    // clean up
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);

    return(buffer.str());
}

string Libxml2Adapter::getSingleNodeContentByXPath(const string xml, const string url, const string xpath)
{
	setXmlDocument(xml, url);
	return(getSingleNodeContentByXPath(xpath));
}


