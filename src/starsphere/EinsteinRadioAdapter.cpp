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

#include "EinsteinRadioAdapter.h"

#include <sstream>

const string EinsteinRadioAdapter::SharedMemoryIdentifier = "EinsteinRadio";

EinsteinRadioAdapter::EinsteinRadioAdapter(BOINCClientAdapter *boincClient) :
	m_WUTemplatePowerSpectrum(POWERSPECTRUM_BINS, 0)
{
	this->boincClient = boincClient;
	m_xmlReader = NULL;

	m_WUSkyPosRightAscension = 0.0;
	m_WUSkyPosDeclination = 0.0;
	m_WUFractionDone = 0.0;
	m_WUCPUTime = 0.0;
}

EinsteinRadioAdapter::~EinsteinRadioAdapter()
{
	if(m_xmlReader) xmlFreeTextReader(m_xmlReader);
    xmlCleanupParser();
}

void EinsteinRadioAdapter::refresh()
{
	boincClient->refresh();
	parseApplicationInformation();
}

void EinsteinRadioAdapter::parseApplicationInformation()
{
	// get updated application information
	string info = boincClient->applicationInformation();

	// do we have any data?
	if(info.length() > 0) {
		int result = 0;

		// prepare conversion stream
		stringstream converter;
		converter.precision(3);
		converter.exceptions(ios_base::badbit | ios_base::failbit);

		if(!m_xmlReader) {
			// set up SAX style XML reader (create instance)
			m_xmlReader = xmlReaderForMemory(info.c_str(),
											 info.length(),
											 "http://einstein.phys.uwm.edu",
											 "UTF-8",
											 0);
			if(!m_xmlReader) {
				cerr << "Error creating XML reader for shared memory data!" << endl;
				return;
			}
		}
		else {
			// set up SAX style XML reader (reusing existing instance)
			if(xmlReaderNewMemory(m_xmlReader,
								  info.c_str(),
								  info.length(),
								  "http://einstein.phys.uwm.edu",
								  "UTF-8",
								  0))
			{
				cerr << "Error updating XML reader for shared memory data!" << endl;
				return;
			}
		}

		// parse XML fragment and process nodes
        result = xmlTextReaderRead(m_xmlReader);
        while (result == 1) {
        	processXmlNode(m_xmlReader, converter);
            result = xmlTextReaderRead(m_xmlReader);
        }

		// convert radians to degrees
		m_WUSkyPosRightAscension *= 180/PI;
		m_WUSkyPosDeclination *= 180/PI;

		// deserialize power spectrum data
		if(m_WUTemplatePowerSpectrumString.length() == POWERSPECTRUM_BIN_BYTES) {
			int spectrumBinValue;
;
			// iterate over all bins
			for(int i = 0, j = 0; i < POWERSPECTRUM_BIN_BYTES; i += 2, ++j) {
				try {
					converter.clear();
					converter.str(m_WUTemplatePowerSpectrumString.substr(i, 2));
					// convert hex bin value to integer
					converter >> hex >> spectrumBinValue;
					// store bin power value
					m_WUTemplatePowerSpectrum.at(j) = (unsigned char) spectrumBinValue;
				}
				catch(ios_base::failure) {
					cerr << "Error processing power spectrum shared memory data!" << endl;
					break;
				}
			}
		}
		else {
			cerr << "Invalid power spectrum shared memory data encountered!" << endl;
		}

	}
}

void EinsteinRadioAdapter::processXmlNode(const xmlTextReaderPtr xmlReader, stringstream& converter)
{
	// we only parse element nodes
    if(xmlTextReaderNodeType(xmlReader) != XML_READER_TYPE_ELEMENT) return;

    // buffers (will be deallocated automatically)
    const xmlChar *nodeName = NULL, *nodeValue = NULL;

    // get element node's name
    nodeName = xmlTextReaderConstLocalName(xmlReader);

    if (nodeName == NULL) {
		cerr << "Error parsing XML node (invalid name)" << endl;
		return;
    }

	// move to node's text content (child node) or return if unavailable
	if(! (xmlTextReaderRead(m_xmlReader) && xmlTextReaderHasValue(xmlReader))) {
		return;
	}

	// get text node's value
	nodeValue = xmlTextReaderConstValue(xmlReader);

	if (nodeValue == NULL) {
		cerr << "Error parsing XML node (invalid value)" << endl;
		return;
	}

    try {
        // prepare converter stream
        converter.clear();
        converter.str("");
        converter << nodeValue;

		// assign node value to respective data member
		if(xmlStrEqual(nodeName, BAD_CAST("skypos_rac"))) {
			converter >> fixed >> m_WUSkyPosRightAscension;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("skypos_dec"))) {
			converter >> fixed >> m_WUSkyPosDeclination;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("dispersion"))) {
			converter >> fixed >> m_WUDispersionMeasure;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("orb_radius"))) {
			converter >> fixed >> m_WUTemplateOrbitalRadius;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("orb_period"))) {
			converter >> fixed >> m_WUTemplateOrbitalPeriod;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("orb_phase"))) {
			converter >> fixed >> m_WUTemplateOrbitalPhase;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("power_spectrum"))) {
			converter >> m_WUTemplatePowerSpectrumString;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("fraction_done"))) {
			converter >> fixed >> m_WUFractionDone;
		}
		else if(xmlStrEqual(nodeName, BAD_CAST("cpu_time"))) {
			converter >> fixed >> m_WUCPUTime;
		}
    }
    catch(ios_base::failure) {
    	cerr << "Error converting XML reader node content!" << endl;
    }
}

double EinsteinRadioAdapter::wuSkyPosRightAscension() const
{
	return m_WUSkyPosRightAscension;
}

double EinsteinRadioAdapter::wuSkyPosDeclination() const
{
	return m_WUSkyPosDeclination;
}

double EinsteinRadioAdapter::wuDispersionMeasure() const
{
	return m_WUDispersionMeasure;
}

double EinsteinRadioAdapter::wuTemplateOrbitalRadius() const
{
	return m_WUTemplateOrbitalRadius;
}

double EinsteinRadioAdapter::wuTemplateOrbitalPeriod() const
{
	return m_WUTemplateOrbitalPeriod;
}

double EinsteinRadioAdapter::wuTemplateOrbitalPhase() const
{
	return m_WUTemplateOrbitalPhase;
}

const vector<unsigned char>* EinsteinRadioAdapter::wuTemplatePowerSpectrum() const
{
	return &m_WUTemplatePowerSpectrum;
}

double EinsteinRadioAdapter::wuFractionDone() const
{
	return m_WUFractionDone;
}

double EinsteinRadioAdapter::wuCPUTime() const
{
	return m_WUCPUTime;
}
