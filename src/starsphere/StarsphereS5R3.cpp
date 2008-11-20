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

#include "StarsphereS5R3.h"

#include <time.h>

StarsphereS5R3::StarsphereS5R3() :
	Starsphere(EinsteinS5R3Adapter::SharedMemoryIdentifier),
	m_EinsteinAdapter(&m_BoincAdapter)
{
	m_CurrentTime = "";
}

StarsphereS5R3::~StarsphereS5R3()
{
}

void StarsphereS5R3::initialize(const int width, const int height, const Resource *font, const bool recycle)
{
	Starsphere::initialize(width, height, font, recycle);

	// check whether we initialize the first time or have to recycle (required for windoze)
	if(!recycle) {

		// adjust HUD config
		m_YOffsetMedium = 15;
		m_XStartPosRight = width - 125;
		m_XStartPosClock = width - 98;
		m_YStartPosBottom = 70;
		m_Y1StartPosBottom = m_YStartPosBottom  - m_YOffsetMedium;
		m_Y2StartPosBottom = m_Y1StartPosBottom - m_YOffsetMedium;
		m_Y3StartPosBottom = m_Y2StartPosBottom - m_YOffsetMedium;
		m_Y4StartPosBottom = m_Y3StartPosBottom - m_YOffsetMedium;
	}

	// create large font instances using font resource (base address + size)
	m_FontLogo1 = new OGLFT::TranslucentTexture(
								&m_FontResource->data()->at(0),
								m_FontResource->data()->size(),
								24, 72 );

	if ( m_FontLogo1 == 0 || !m_FontLogo1->isValid() ) {
	     cerr << "Could not construct logo1 font face from in memory resource!" << endl;
	     return;
	}

	m_FontLogo1->setForegroundColor(1.0, 1.0, 0.0, 1.0);

	// create medium font instances using font resource (base address + size)
	m_FontLogo2 = new OGLFT::TranslucentTexture(
								&m_FontResource->data()->at(0),
								m_FontResource->data()->size(),
								13, 78 );

	if ( m_FontLogo2 == 0 || !m_FontLogo2->isValid() ) {
	     cerr << "Could not construct logo2 font face from in memory resource!" << endl;
	     return;
	}

	m_FontLogo2->setForegroundColor(0.75, 0.75, 0.75, 1.0);

	// prepare base class observatories (not dimmed)
	generateObservatories(1.0);
}

void StarsphereS5R3::resize(const int width, const int height)
{
	Starsphere::resize(width, height);

	// adjust HUD config
	m_XStartPosRight = width - 125;
	m_XStartPosClock = width - 98;
}

void StarsphereS5R3::refreshBOINCInformation()
{
	// call base class implementation
	Starsphere::refreshLocalBOINCInformation();

	// update local/specific content
	m_EinsteinAdapter.refresh();

	// prepare conversion buffer
	stringstream buffer;
	buffer.precision(2);
	buffer.setf(ios::fixed, ios::floatfield);
	buffer.fill('0');
	buffer.setf(ios::right, ios::adjustfield);

	// store content required for our HUD (search info)
	if(m_CurrentRightAscension != m_EinsteinAdapter.wuSkyPosRightAscension()) {
		// we've got a new position, update search marker and HUD
		m_CurrentRightAscension = m_EinsteinAdapter.wuSkyPosRightAscension();
		m_RefreshSearchMarker = true;
		buffer.str("");
		buffer << "Ascension: " << fixed << m_CurrentRightAscension << " deg" << ends;
		m_WUSkyPosRightAscension = buffer.str();
	}

	if(m_CurrentDeclination != m_EinsteinAdapter.wuSkyPosDeclination()) {
		// we've got a new position, update search marker and HUD
		m_CurrentDeclination = m_EinsteinAdapter.wuSkyPosDeclination();
		m_RefreshSearchMarker = true;
		buffer.str("");
		buffer << "Declination: " << fixed << m_CurrentDeclination << " deg" << ends;
		m_WUSkyPosDeclination = buffer.str();
	}

	buffer.str("");
	buffer << "Completed: " << fixed << m_EinsteinAdapter.wuFractionDone() * 100 << " %" << ends;
	m_WUPercentDone = buffer.str();

	// show WU's total CPU time (previously accumulated + current session)
	double timeCPU = m_BoincAdapter.wuCPUTimeSpent() + m_EinsteinAdapter.wuCPUTime();
	int hrs = timeCPU / 3600;
	int min = fmod(timeCPU, 3600) / 60;
	int sec = fmod(timeCPU, 60);

	buffer.str("");
	buffer << "CPU Time: "  << right << setw(2) << hrs << ":"
							<< right << setw(2) << min << ":"
							<< right << setw(2) << sec << ends;

	m_WUCPUTime = buffer.str();

	// update current time string (clock)
	char cBuffer[10] = {0};
	time_t timeNow = time(0);
	struct tm* timeLocal = localtime(&timeNow);
	strftime(cBuffer, sizeof(cBuffer) - 1, "%H:%M:%S", timeLocal);

	m_CurrentTime = string(cBuffer);
}

void StarsphereS5R3::renderSearchInformation()
{
		// clock
		m_FontLogo1->draw(m_XStartPosClock, m_YStartPosTop, m_CurrentTime.c_str());

		// left info block
		m_FontHeader->draw(m_XStartPosLeft, m_YStartPosBottom, "BOINC Statistics");
		m_FontText->draw(m_XStartPosLeft, m_Y1StartPosBottom, m_UserName.c_str());
		m_FontText->draw(m_XStartPosLeft, m_Y2StartPosBottom, m_TeamName.c_str());
		m_FontText->draw(m_XStartPosLeft, m_Y3StartPosBottom, m_UserCredit.c_str());
		m_FontText->draw(m_XStartPosLeft, m_Y4StartPosBottom, m_UserRACredit.c_str());

		// right info block
		m_FontHeader->draw(m_XStartPosRight, m_YStartPosBottom, "Search Information");
		m_FontText->draw(m_XStartPosRight, m_Y1StartPosBottom, m_WUSkyPosRightAscension.c_str());
		m_FontText->draw(m_XStartPosRight, m_Y2StartPosBottom, m_WUSkyPosDeclination.c_str());
		m_FontText->draw(m_XStartPosRight, m_Y3StartPosBottom, m_WUPercentDone.c_str());
		m_FontText->draw(m_XStartPosRight, m_Y4StartPosBottom, m_WUCPUTime.c_str());
}

void StarsphereS5R3::generateObservatories(float dimFactor)
{
	// we don't do anything special here, just call base class
	Starsphere::generateObservatories(dimFactor);
}

void StarsphereS5R3::renderLogo()
{
	m_FontLogo1->draw(m_XStartPosLeft, m_YStartPosTop, "Einstein@Home");
	m_FontLogo2->draw(m_XStartPosLeft, m_YStartPosTop - m_YOffsetLarge, "World Year of Physics 2005");
}
