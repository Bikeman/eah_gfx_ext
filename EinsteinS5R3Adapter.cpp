#include "EinsteinS5R3Adapter.h"

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
