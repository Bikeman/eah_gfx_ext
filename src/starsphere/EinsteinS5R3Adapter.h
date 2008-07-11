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

#ifndef EINSTEINS5R3ADAPTER_H_
#define EINSTEINS5R3ADAPTER_H_

#include <string>

#include "BOINCClientAdapter.h"

using namespace std;

#define PI 3.14159265

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief Adapter class which facilitates communications with the \b Einstein\@Home S5R3 application
 *
 * This adapter class can be used to query the \b Einstein\@Home S5R3 application
 * for informational data about the current work unit like search details and progress.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class EinsteinS5R3Adapter
{
public:
	/**
	 * \brief Constructor
	 *
	 * \param boincClient Pointer to the parent BOINC client adapter instance
	 */
	EinsteinS5R3Adapter(BOINCClientAdapter* boincClient);

	/// Destructor
	virtual ~EinsteinS5R3Adapter();

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

    /// The identifier of the Einstein\@Home science application's shared memory area
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

	/// Pointer to the (parent) BOINC client adapter
	BOINCClientAdapter *boincClient;

	/// Right ascension of the currently searched sky position (in degrees)
	double m_WUSkyPosRightAscension;

	/// Declination of the currently searched sky position (in degrees)
	double m_WUSkyPosDeclination;

	/// The completion fraction of the active work unit
	double m_WUFractionDone;

	/// Amount of CPU time consumed for the work unit during the active session
	double m_WUCPUTime;
};

/**
 * @}
 */

#endif /*EINSTEINS5R3ADAPTER_H_*/
