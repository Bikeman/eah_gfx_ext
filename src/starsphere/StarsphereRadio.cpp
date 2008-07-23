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

#include "StarsphereRadio.h"

StarsphereRadio::StarsphereRadio() : Starsphere(), m_EinsteinAdapter(&m_BoincAdapter)
{
	m_WUDispersionMeasureValue = -1.0;
	m_PowerSpectrumCoordSystemList = 0;
	m_PowerSpectrumFreqBins = new vector<char>(40, 0);
}

StarsphereRadio::~StarsphereRadio()
{
	if(m_PowerSpectrumFreqBins) delete m_PowerSpectrumFreqBins;
}

void StarsphereRadio::initialize(const int width, const int height, const Resource *font, const bool recycle)
{
	Starsphere::initialize(width, height, font, recycle);

	// check whether we initialize the first time or have to recycle (required for windoze)
	if(!recycle) {

		// adjust HUD config
		m_YOffsetMedium = 15;
		m_XStartPosRight = width - 125;
		m_YStartPosBottom = 100;
		m_Y1StartPosBottom = m_YStartPosBottom  - m_YOffsetMedium;
		m_Y2StartPosBottom = m_Y1StartPosBottom - m_YOffsetMedium;
		m_Y3StartPosBottom = m_Y2StartPosBottom - m_YOffsetMedium;
		m_Y4StartPosBottom = m_Y3StartPosBottom - m_YOffsetMedium;
		m_Y5StartPosBottom = m_Y4StartPosBottom - m_YOffsetMedium;
		m_Y6StartPosBottom = m_Y5StartPosBottom - m_YOffsetMedium;
	}

	// prepare power spectrum
	// TODO: store fixed values as class consts
	generatePowerSpectrumCoordSystem(width - 210, height - 60);
}

void StarsphereRadio::resize(const int width, const int height)
{
	Starsphere::resize(width, height);

	// adjust HUD config
	// TODO: store fixed values as class consts
	m_XStartPosRight = width - 125;

	// update static power spectrum parts
	// TODO: store fixed values as class consts
	generatePowerSpectrumCoordSystem(width - 210, height - 60);
}

void StarsphereRadio::refreshBOINCInformation()
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
		buffer << "Ascension: " << fixed << m_CurrentRightAscension << " deg" << ends;
		m_WUSkyPosRightAscension = buffer.str();
		buffer.str("");
	}

	if(m_CurrentDeclination != m_EinsteinAdapter.wuSkyPosDeclination()) {
		// we've got a new position, update search marker and HUD
		m_CurrentDeclination = m_EinsteinAdapter.wuSkyPosDeclination();
		m_RefreshSearchMarker = true;
		buffer << "Declination: " << fixed << m_CurrentDeclination << " deg" << ends;
		m_WUSkyPosDeclination = buffer.str();
		buffer.str("");
	}

	if(m_WUDispersionMeasureValue != m_EinsteinAdapter.wuDispersionMeasure()) {
		// we've got a new dispersion measure, update HUD
		m_WUDispersionMeasureValue = m_EinsteinAdapter.wuSkyPosDeclination();
		buffer << "DM: " << fixed << m_WUDispersionMeasureValue << " pc/cm3" << ends;
		m_WUDispersionMeasure = buffer.str();
		buffer.str("");
	}

	// update the following information every time (no need to check first)

	buffer.precision(3);
	buffer << "Orb. Radius: " << fixed << m_EinsteinAdapter.wuTemplateOrbitalRadius() << " ls" << ends;
	m_WUTemplateOrbitalRadius = buffer.str();
	buffer.str("");

	buffer.precision(0);
	buffer << "Orb. Period: " << fixed << m_EinsteinAdapter.wuTemplateOrbitalPeriod() << " s" << ends;
	m_WUTemplateOrbitalPeriod = buffer.str();
	buffer.str("");

	buffer.precision(2);
	buffer << "Orb. Phase: " << fixed << m_EinsteinAdapter.wuTemplateOrbitalPhase() << " rad" << ends;
	m_WUTemplateOrbitalPhase = buffer.str();
	buffer.str("");

	buffer << "WU Completed: " << fixed << m_EinsteinAdapter.wuFractionDone() * 100 << " %" << ends;
	m_WUPercentDone = buffer.str();
	buffer.str("");

	// show WU's total CPU time (previously accumulated + current session)
	const double cputime = m_BoincAdapter.wuCPUTimeSpent() + m_EinsteinAdapter.wuCPUTime();
	const int hrs =  cputime / 3600;
	const int min = (cputime - hrs*3600) / 60;
	const int sec =  cputime - (hrs*3600 + min*60);

	buffer << "WU CPU Time: "  << right << setw(2) << hrs << ":"
							<< right << setw(2) << min << ":"
							<< right << setw(2) << sec << ends;

	m_WUCPUTime = buffer.str();

	// update power spectrum bin data
	// TODO: store fixed values as class consts
	// TODO: this has to be a separate method!
	generatePowerSpectrumCoordSystem(m_CurrentWidth - 210, m_CurrentHeight - 60);
}

void StarsphereRadio::renderSearchInformation()
{
	// left info block
	m_FontHeader->draw(m_XStartPosLeft, m_YStartPosBottom, "BOINC Information");
	m_FontText->draw(m_XStartPosLeft, m_Y1StartPosBottom, m_UserName.c_str());
	m_FontText->draw(m_XStartPosLeft, m_Y2StartPosBottom, m_TeamName.c_str());
	m_FontText->draw(m_XStartPosLeft, m_Y3StartPosBottom, m_UserCredit.c_str());
	m_FontText->draw(m_XStartPosLeft, m_Y4StartPosBottom, m_UserRACredit.c_str());
	m_FontText->draw(m_XStartPosLeft, m_Y5StartPosBottom, m_WUPercentDone.c_str());
	m_FontText->draw(m_XStartPosLeft, m_Y6StartPosBottom, m_WUCPUTime.c_str());

	// right info block
	m_FontHeader->draw(m_XStartPosRight, m_YStartPosBottom, "Search Information");
	m_FontText->draw(m_XStartPosRight, m_Y1StartPosBottom, m_WUSkyPosRightAscension.c_str());
	m_FontText->draw(m_XStartPosRight, m_Y2StartPosBottom, m_WUSkyPosDeclination.c_str());
	m_FontText->draw(m_XStartPosRight, m_Y3StartPosBottom, m_WUDispersionMeasure.c_str());
	m_FontText->draw(m_XStartPosRight, m_Y4StartPosBottom, m_WUTemplateOrbitalRadius.c_str());
	m_FontText->draw(m_XStartPosRight, m_Y5StartPosBottom, m_WUTemplateOrbitalPeriod.c_str());
	m_FontText->draw(m_XStartPosRight, m_Y6StartPosBottom, m_WUTemplateOrbitalPhase.c_str());

	// power spectrum
	renderPowerSpectrum();
}

void StarsphereRadio::renderPowerSpectrum()
{
	// TODO: store fixed values as class consts
	m_FontText->draw(m_XStartPosRight - 20, m_CurrentHeight - 75, "Power Spectrum");

	glPushMatrix();
	glLoadIdentity();
	glCallList(m_PowerSpectrumCoordSystemList);
	//TODO: separate call list for bins!
	glPopMatrix();
}

void StarsphereRadio::generatePowerSpectrumCoordSystem(const int originX, const int originY)
{
	GLfloat offsetX = (GLfloat)originX;
	GLfloat offsetY = (GLfloat)originY;
	GLfloat axesWidth = 2.0;
	GLfloat binWidth = 5.0;

	// delete existing, create new (required for windoze)
	if(m_PowerSpectrumCoordSystemList) glDeleteLists(m_PowerSpectrumCoordSystemList, 1);
	m_PowerSpectrumCoordSystemList = glGenLists(1);
	glNewList(m_PowerSpectrumCoordSystemList, GL_COMPILE);

		// draw coordinate system axes
		glBegin(GL_LINE_STRIP);
			glLineWidth(axesWidth);
			glColor4f(0.7, 0.7, 0.0, 1.0);
			glVertex2f(offsetX, offsetY + 50.0);
			glVertex2f(offsetX, offsetY);
			glVertex2f(offsetX + 200, offsetY);
		glEnd();

		// draw origin (axes joint)
		glBegin(GL_POINTS);
			glPointSize(axesWidth);
			glColor4f(0.7, 0.7, 0.0, 1.0);
			glVertex2f(offsetX, offsetY);
		glEnd();

		//TODO: for high quality mode: draw coord. system backdrop with alpha = ~0.3

//		------test code
		srand(time(NULL));
		for(int i=0; i<40; ++i) {
			m_PowerSpectrumFreqBins->at(i) = rand()%10;
		}

		// draw frequency bins
		glBegin(GL_LINES);
			glLineWidth(binWidth);
			glColor4f(1.0, 1.0, 1.0, 1.0);

			// iterate over all bins
			for(int i=0; i<40; ++i) {
				glVertex2f(offsetX+1+i*binWidth, offsetY+1);
				glVertex2f(offsetX+1+i*binWidth, offsetY+1+m_PowerSpectrumFreqBins->at(i)*4.5);
			}
		glEnd();
//		------test code


	glEndList();
}
