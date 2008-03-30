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
	
	string getCoreVersion() const;
	string getApplicationName() const;
	string getApplicationVersion() const;
	
	string getUserName() const;
	string getTeamName() const;
	double getUserCredit() const;
	double getUserRACredit() const;
	double getHostCredit() const;
	double getHostRACredit() const;
	
	string getWUName() const;
	double getWUCPUTime() const;
    double getWUFPOpsEstimated() const;
    double getWUFPOpsBound() const;
    double getWUMemoryBound() const;
    double getWUDiskBound() const;
	
private:
	void updateUserInfo();
	void updateSharedMemoryArea();
	
	char *m_SharedMemoryArea;
	bool m_SharedMemoryAreaAvailable;
	APP_INIT_DATA m_UserData;
	
	float m_SkyPosRightAscension;
	float m_SkyPosDeclination;
	float m_WUFractionDone;
};

#endif /*BOINCCLIENTADAPTER_H_*/
