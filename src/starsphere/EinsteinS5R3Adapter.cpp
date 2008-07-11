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

#include "EinsteinS5R3Adapter.h"

const string EinsteinS5R3Adapter::SharedMemoryIdentifier = "EinsteinHS";

EinsteinS5R3Adapter::EinsteinS5R3Adapter(BOINCClientAdapter *boincClient)
{
	this->boincClient = boincClient;

	m_WUSkyPosRightAscension = 0.0;
	m_WUSkyPosDeclination = 0.0;
	m_WUFractionDone = 0.0;
	m_WUCPUTime = 0.0;
}

EinsteinS5R3Adapter::~EinsteinS5R3Adapter()
{
}

void EinsteinS5R3Adapter::refresh()
{
	boincClient->refresh();
	parseApplicationInformation();
}

void EinsteinS5R3Adapter::parseApplicationInformation()
{
	// get updated application information
	string info = boincClient->applicationInformation();

	// do we have any data?
	if(info.length() > 0) {

		// parse data into members
		if(4 != sscanf(info.c_str(),
			  		"<graphics_info>\n"
			  		"  <skypos_rac>%lf</skypos_rac>\n"
			  		"  <skypos_dec>%lf</skypos_dec>\n"
			  		"  <fraction_done>%lf</fraction_done>\n"
			  		"  <cpu_time>%lf</cpu_time>\n",
			  		&m_WUSkyPosRightAscension,
			  		&m_WUSkyPosDeclination,
			  		&m_WUFractionDone,
			  		&m_WUCPUTime))
		{
			cerr << "Incompatible shared memory data encountered!" << endl;
		}
		else {
			// convert radians to degrees
			m_WUSkyPosRightAscension *= 180/PI;
			m_WUSkyPosDeclination *= 180/PI;
		}
	}
}

double EinsteinS5R3Adapter::wuSkyPosRightAscension() const
{
	return m_WUSkyPosRightAscension;
}

double EinsteinS5R3Adapter::wuSkyPosDeclination() const
{
	return m_WUSkyPosDeclination;
}

double EinsteinS5R3Adapter::wuFractionDone() const
{
	return m_WUFractionDone;
}

double EinsteinS5R3Adapter::wuCPUTime() const
{
	return m_WUCPUTime;
}
