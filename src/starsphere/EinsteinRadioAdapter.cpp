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

const string EinsteinRadioAdapter::SharedMemoryIdentifier = "EinsteinRadio";

EinsteinRadioAdapter::EinsteinRadioAdapter(BOINCClientAdapter *boincClient) :
	m_WUTemplatePowerSpectrum(POWERSPECTRUM_BINS, 0)
{
	this->boincClient = boincClient;

	m_WUSkyPosRightAscension = 0.0;
	m_WUSkyPosDeclination = 0.0;
	m_WUFractionDone = 0.0;
	m_WUCPUTime = 0.0;
}

EinsteinRadioAdapter::~EinsteinRadioAdapter()
{
}

void EinsteinRadioAdapter::refresh()
{
	boincClient->refresh();
	parseApplicationInformation();
}

void EinsteinRadioAdapter::parseApplicationInformation()
{
	char spectrumArray[POWERSPECTRUM_BIN_BYTES + 1] = {0};

	// get updated application information
	string info = boincClient->applicationInformation();

	// do we have any data?
	if(info.length() > 0) {

		// parse data into members
		// TODO: this is soon going to be replaced by true XML parsing!
		if(9 != sscanf(info.c_str(),
				"<graphics_info>\n"
				"  <skypos_rac>%lf</skypos_rac>\n"
				"  <skypos_dec>%lf</skypos_dec>\n"
				"  <dispersion>%lf</dispersion>\n"
				"  <orb_radius>%lf</orb_radius>\n"
				"  <orb_period>%lf</orb_period\n"
				"  <orb_phase>%lf</orb_phase>\n"
				"  <power_spectrum>%80c</power_spectrum>\n"
				"  <fraction_done>%lf</fraction_done>\n"
				"  <cpu_time>%lf</cpu_time>\n",
				&m_WUSkyPosRightAscension,
				&m_WUSkyPosDeclination,
				&m_WUDispersionMeasure,
				&m_WUTemplateOrbitalRadius,
				&m_WUTemplateOrbitalPeriod,
				&m_WUTemplateOrbitalPhase,
				spectrumArray,
				&m_WUFractionDone,
				&m_WUCPUTime))
		{
			cerr << "Incompatible shared memory data encountered!" << endl;
		}
		else {
			// convert radians to degrees
			m_WUSkyPosRightAscension *= 180/PI;
			m_WUSkyPosDeclination *= 180/PI;

			// deserialize power spectrum data
			string spectrumString(spectrumArray);
			if(spectrumString.length() == POWERSPECTRUM_BIN_BYTES) {
				int spectrumBinValue;
				// prepare hex to int conversion stream
				istringstream spectrumBinStream;
				spectrumBinStream.exceptions(ios_base::badbit | ios_base::failbit);
				// iterate over all bins
				for(int i = 0, j = 0; i < POWERSPECTRUM_BIN_BYTES; i += 2, ++j) {
					try {
						spectrumBinStream.str(spectrumString.substr(i, 2));
						// convert hex bin value to integer
						spectrumBinStream >> hex >> spectrumBinValue;
						// store bin power value
						m_WUTemplatePowerSpectrum.at(j) = (unsigned char) spectrumBinValue;
						spectrumBinStream.clear();
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
