#include "StarsphereS5R3.h"

StarsphereS5R3::StarsphereS5R3() : m_EinsteinAdapter(&m_BoincAdapter)
{
	m_YStartPosBottom = 0.07;
}

StarsphereS5R3::~StarsphereS5R3()
{
}

void StarsphereS5R3::refreshBOINCInformation()
{
	Starsphere::refreshBOINCInformation();
	
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
		buffer << "Ascension: " << fixed << m_CurrentRightAscension * 360/PI2 << " deg" << ends;
		m_WUSkyPosRightAscension = buffer.str();
		buffer.str("");
	}
	
	if(m_CurrentDeclination != m_EinsteinAdapter.wuSkyPosDeclination()) {
		// we've got a new position, update search marker and HUD
		m_CurrentDeclination = m_EinsteinAdapter.wuSkyPosDeclination();
		m_RefreshSearchMarker = true;
		buffer << "Declination: " << fixed << m_CurrentDeclination * 360/PI2 << " deg" << ends;
		m_WUSkyPosDeclination = buffer.str();
		buffer.str("");
	}
	
	buffer << "Completed: " << fixed << m_EinsteinAdapter.wuFractionDone() * 100 << " %" << ends;
	m_WUPercentDone = buffer.str();
	buffer.str("");
	
	const double cputime = m_EinsteinAdapter.wuCPUTime();
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
		m_XStartPosRight = 1 * aspect - 0.145;
	
		// left info block      
		glPushMatrix();
		
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(m_XStartPosLeft, m_YStartPosBottom, 0);
		glScalef(m_FontScaleMedium, m_FontScaleMedium, 1.0);
		m_PolygonFont->Render("BOINC Statistics");
	
		glLoadIdentity();
		glColor4f(1.0, 1.0, 1.0, 0.5);
		glTranslatef(m_XStartPosLeft, m_YStartPosBottom - m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_UserName.c_str());
	
		glLoadIdentity();
		glTranslatef(m_XStartPosLeft, m_YStartPosBottom - 2*m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_TeamName.c_str());
	
		glLoadIdentity();
		glTranslatef(m_XStartPosLeft, m_YStartPosBottom - 3*m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_UserCredit.c_str());
	
		glLoadIdentity();
		glTranslatef(m_XStartPosLeft, m_YStartPosBottom - 4*m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_UserRACredit.c_str());
		
		glPopMatrix();
	
		// right info block
		glPushMatrix();
		
		glColor3f(1.0, 1.0, 0.0);
		glTranslatef(m_XStartPosRight, m_YStartPosBottom, 0);
		glScalef(m_FontScaleMedium, m_FontScaleMedium, 1.0);
		m_PolygonFont->Render("Search Information");
	
		glLoadIdentity();
		glColor4f(1.0, 1.0, 1.0, 0.5);
		glTranslatef(m_XStartPosRight, m_YStartPosBottom - m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_WUSkyPosRightAscension.c_str());
	
		glLoadIdentity();
		glTranslatef(m_XStartPosRight, m_YStartPosBottom - 2*m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_WUSkyPosDeclination.c_str());
	
		glLoadIdentity();
		glTranslatef(m_XStartPosRight, m_YStartPosBottom - 3*m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_WUPercentDone.c_str());
	
		glLoadIdentity();
		glTranslatef(m_XStartPosRight, m_YStartPosBottom - 4*m_YOffsetMedium, 0);
		glScalef(m_FontScaleSmall, m_FontScaleSmall, 1.0);
		m_PolygonFont->Render(m_WUCPUTime.c_str());
		
		glPopMatrix();
}
