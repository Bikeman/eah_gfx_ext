#ifndef STARSPHERES5R3_H_
#define STARSPHERES5R3_H_

#include <sstream>

#include "Starsphere.h"
#include "EinsteinS5R3Adapter.h"

using namespace std;

class StarsphereS5R3 : public Starsphere
{
public:
	StarsphereS5R3();
	virtual ~StarsphereS5R3();
	
	void refreshBOINCInformation();
	
private:
	void renderSearchInformation();
	
	EinsteinS5R3Adapter m_EinsteinAdapter;
	
	string m_WUSkyPosRightAscension;
	string m_WUSkyPosDeclination;
	string m_WUPercentDone;
	string m_WUCPUTime;
	
	GLfloat m_XStartPosRight;
	GLfloat m_YStartPosBottom;
};

#endif /*STARSPHERES5R3_H_*/
