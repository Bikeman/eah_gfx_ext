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

#ifndef EINSTEINRADIOADAPTER_H_
#define EINSTEINRADIOADAPTER_H_

#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

#include <libxml/xmlreader.h>

#include "BOINCClientAdapter.h"

using namespace std;

#define PI 3.14159265
#define POWERSPECTRUM_BINS 40
#define POWERSPECTRUM_BIN_BYTES 80

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief Adapter class which facilitates communications with the \b Einstein\@Home Radio Pulsar application
 *
 * This adapter class can be used to query the \b Einstein\@Home Radio Pulsar application
 * for informational data about the current work unit like search details and progress.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class EinsteinRadioAdapter
{
public:
	/**
	 * \brief Constructor
	 *
	 * \param boincClient Pointer to the parent BOINC client adapter instance
	 */
	EinsteinRadioAdapter(BOINCClientAdapter* boincClient);

	/// Destructor
	virtual ~EinsteinRadioAdapter();

	/**
	 * \brief Refreshes dynamic data (e.g. search information)
	 *
	 * You want to call this method periodically to refresh any volatile application information
	 *
	 * \see AbstractGraphicsEngine::refreshBOINCInformation
	 */
	void refresh();

    /**
	 * \brief Retrieves the right ascension of the currently searched sky position
	 *
	 * \return The right ascension (in degrees)
	 */
    double wuSkyPosRightAscension() const;

    /**
	 * \brief Retrieves the declination of the currently searched sky position
	 *
	 * \return The right ascension (in degrees)
	 */
    double wuSkyPosDeclination() const;

    /**
	 * \brief Retrieves the dispersion measure of the currently searched sky position
	 *
	 * \return The dispersion measure
	 */
	double wuDispersionMeasure() const;

    /**
	 * \brief Retrieves the projected orbital radius of the currently active template
	 *
	 * \return The projected orbital radius
	 */
	double wuTemplateOrbitalRadius() const;

    /**
	 * \brief Retrieves the orbital period of the currently active template
	 *
	 * \return The orbital period of the currently active template
	 */
	double wuTemplateOrbitalPeriod() const;

    /**
	 * \brief Retrieves the initial orbital phase of the currently active template
	 *
	 * \return The initial orbital phase of the currently active template
	 */
	double wuTemplateOrbitalPhase() const;

    /**
	 * \brief Retrieves the power spectrum of the currently active template
	 *
	 * \return The power spectrum of the currently active template
	 */
	const vector<unsigned char>* wuTemplatePowerSpectrum() const;

    /**
     * \brief Retrieves the completion fraction of the currently active work unit
     *
     * \return The completion fraction (range 0-1)
     */
    double wuFractionDone() const;

    /**
     * \brief Retrieves the amount of CPU time consumed for the currently active work unit
     * during the active session
     *
     * \return The accumulated CPU time consumed during this work unit session (in seconds)
     */
    double wuCPUTime() const;

    /// The identifier of the Einstein\@Home Radio Pulsar science application's shared memory area
    static const string SharedMemoryIdentifier;

private:

	/**
	 * \brief Parses science application specific information into local attributes
	 *
	 * The information is usually transferred via a shared memory area
	 * which is handled by the parent generic BOINC client adapter.
	 *
	 * \see boincClient
	 */
	void parseApplicationInformation();

	/**
	 * \brief Processes single element nodes found by the XML parser
	 *
	 * This method can sort of resembles the callback method used in conjunction with SAX parsers.
	 * It's called to process single XML nodes (in our case element nodes only) encountered
	 * during XML document (tree) traversal.
	 *
	 * \param xmlReader The pointer to the current xmlReader instance
	 * \param converter Reference to an existing string stream for value/type conversion
	 *
	 * \see parseApplicationInformation()
	 */
	void processXmlNode(const xmlTextReaderPtr xmlReader, stringstream& converter);

	/// Pointer to the (parent) BOINC client adapter
	BOINCClientAdapter *boincClient;

	/// Pointer to the XML reader (SAX style)
	xmlTextReaderPtr m_xmlReader;

	/// Right ascension of the currently searched sky position (in degrees)
	double m_WUSkyPosRightAscension;

	/// Declination of the currently searched sky position (in degrees)
	double m_WUSkyPosDeclination;

	/// Dispersion measure of the currently searched sky position
	double m_WUDispersionMeasure;

	/// Projected orbital radius of the currently active template
	double m_WUTemplateOrbitalRadius;

	/// Orbital period of the currently active template
	double m_WUTemplateOrbitalPeriod;

	/// Initial orbital phase of the currently active template
	double m_WUTemplateOrbitalPhase;

	/// Power spectrum of the currently active template (string)
	string m_WUTemplatePowerSpectrumString;

	/// Power spectrum of the currently active template (values)
	vector<unsigned char> m_WUTemplatePowerSpectrum;

	/// The completion fraction of the active work unit
	double m_WUFractionDone;

	/// Amount of CPU time consumed for the work unit during the active session
	double m_WUCPUTime;
};

/**
 * @}
 */

#endif /*EINSTEINRADIOADAPTER_H_*/
