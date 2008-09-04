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
	if(m_xmlDocument) xmlFreeDoc(m_xmlDocument);
	xmlCleanupParser();
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

	string result = "";

	// prepare xpath context
    xmlXPathContextPtr xpathCtx = xmlXPathNewContext(m_xmlDocument);
    if(xpathCtx == NULL) {
    	cerr << "Error getting XPath context!" << endl;
    	return("");
    }

    // prepare xpath expression
    xmlChar* xpathExpr = xmlCharStrdup(xpath.c_str());
    if(xpathExpr == NULL) {
    	cerr << "Error preparing XPath expression: " << xpath << endl;
    	xmlXPathFreeContext(xpathCtx);
    	return("");
    }

    // run xpath query
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(xpathExpr, xpathCtx);
    if(xpathObj == NULL) {
    	cerr << "Error evaluating XPath expression: " << xpath << endl;
    	xmlFree(xpathExpr);
    	xmlXPathFreeContext(xpathCtx);
    	return("");
    }

    // retrieve node set returned by xpath query
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    // how many nodes did we find?
    int size = (nodes) ? nodes->nodeNr : 0;

    if(size <= 0) {
    	cerr << "No node found using XPath expression: " << xpath << endl;
    	result = "";
    }
    else if(size > 1) {
    	cerr << "More than node found using XPath expression: " << xpath << endl;
    	result = "";
    }
    else {
		// prepare conversion stream
		ostringstream converter;
		converter.exceptions(ios_base::badbit | ios_base::failbit);

		// get xml content
		xmlChar* nodeContent = xmlNodeListGetString(m_xmlDocument, nodes->nodeTab[0]->xmlChildrenNode, 1);

		try {
			// convert xml contents
			converter << nodeContent;
			result = converter.str();
		}
		catch(ios_base::failure) {
			cerr << "Error converting XPath node content!" << endl;
			result = "";
		}

		// clean up
		xmlFree(nodeContent);
    }

    // clean up
    xmlFree(xpathExpr);
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);

    return(result);
}

string Libxml2Adapter::getSingleNodeContentByXPath(const string xml, const string url, const string xpath)
{
	setXmlDocument(xml, url);
	return(getSingleNodeContentByXPath(xpath));
}


