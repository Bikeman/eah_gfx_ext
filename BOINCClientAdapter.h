#ifndef BOINCCLIENTADAPTER_H_
#define BOINCCLIENTADAPTER_H_

#include <string>

#include "boinc_api.h"
#include "graphics2.h"

#define EAH_SHMEM_APP_NAME "EinsteinHS"

class BOINCClientAdapter
{
public:
	BOINCClientAdapter();
	virtual ~BOINCClientAdapter();
	
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
	double wuCPUTime() const;
    double wuFPOpsEstimated() const;
    double wuFPOpsBound() const;
    double wuMemoryBound() const;
    double wuDiskBound() const;
	
private:
	void readUserInfo();
	void readSharedMemoryArea();
	
	char *m_SharedMemoryArea;
	bool m_SharedMemoryAreaAvailable;
	APP_INIT_DATA m_UserData;
	
	float m_SkyPosRightAscension;
	float m_SkyPosDeclination;
	float m_WUFractionDone;
};

#endif /*BOINCCLIENTADAPTER_H_*/
