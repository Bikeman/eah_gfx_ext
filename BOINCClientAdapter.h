#ifndef BOINCCLIENTADAPTER_H_
#define BOINCCLIENTADAPTER_H_

#include <iostream>
#include <string>
#include <sstream>

#include "boinc_api.h"
#include "graphics2.h"

using namespace std;

#define EAH_SHMEM_APP_NAME "EinsteinHS"

class BOINCClientAdapter
{
public:
	BOINCClientAdapter();
	virtual ~BOINCClientAdapter();
	
	void refresh();
	
	string coreVersion() const;
	string applicationName() const;
	string applicationVersion() const;
	
	string userName() const;
	string teamName() const;
	double userCredit() const;
	double userRACredit() const;
	double hostCredit() const;
	double hostRACredit() const;
	
	string wuName() const;
    double wuFPOpsEstimated() const;
    double wuFPOpsBound() const;
    double wuMemoryBound() const;
    double wuDiskBound() const;
    
    double wuSkyPosRightAscension() const;
    double wuSkyPosDeclination() const;
    double wuFractionDone() const;
    double wuCPUTime() const;
	
private:
	void readUserInfo();
	void readSharedMemoryArea();
	
	char *m_SharedMemoryArea;
	bool m_SharedMemoryAreaAvailable;
	APP_INIT_DATA m_UserData;
	
	double m_WUSkyPosRightAscension;
	double m_WUSkyPosDeclination;
	double m_WUFractionDone;
	double m_WUCPUTime;
};

#endif /*BOINCCLIENTADAPTER_H_*/
