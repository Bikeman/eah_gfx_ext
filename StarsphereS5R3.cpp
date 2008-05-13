#include "StarsphereS5R3.h"

StarsphereS5R3::StarsphereS5R3() : Starsphere(), m_EinsteinAdapter(&m_BoincAdapter)
{
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
		m_YStartPosBottom = 70;	
		m_Y1StartPosBottom = m_YStartPosBottom  - m_YOffsetMedium;
		m_Y2StartPosBottom = m_Y1StartPosBottom - m_YOffsetMedium;
		m_Y3StartPosBottom = m_Y2StartPosBottom - m_YOffsetMedium;
		m_Y4StartPosBottom = m_Y3StartPosBottom - m_YOffsetMedium;
	}
}

void StarsphereS5R3::resize(const int width, const int height)
{
	Starsphere::resize(width, height);
	
	// adjust HUD config
	m_XStartPosRight = width - 125;
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
	
	buffer << "Completed: " << fixed << m_EinsteinAdapter.wuFractionDone() * 100 << " %" << ends;
	m_WUPercentDone = buffer.str();
	buffer.str("");
	
	// show WU's total CPU time (previously accumulated + current session) 
	const double cputime = m_BoincAdapter.wuCPUTimeSpent() + m_EinsteinAdapter.wuCPUTime();
	const int hrs =  cputime / 3600;
	const int min = (cputime - hrs*3600) / 60;
	const int sec =  cputime - (hrs*3600 + min*60);	

	buffer << "CPU Time: "  << right << setw(2) << hrs << ":"
							<< right << setw(2) << min << ":"
							<< right << setw(2) << sec << ends;
	
	m_WUCPUTime = buffer.str();	
}

void StarsphereS5R3::renderSearchInformation()
{
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
