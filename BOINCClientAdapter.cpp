#include "BOINCClientAdapter.h"

BOINCClientAdapter::BOINCClientAdapter()
{
	updateUserInfo();
	
	m_SharedMemoryArea = (char*) boinc_graphics_get_shmem(EAH_SHMEM_APP_NAME);
	
	if(m_SharedMemoryArea) {
		updateSharedMemoryArea();
		m_SharedMemoryAreaAvailable = true;
	}
	else {
		m_SharedMemoryAreaAvailable = false;
	}
}

BOINCClientAdapter::~BOINCClientAdapter()
{
}

void BOINCClientAdapter::updateUserInfo()
{
	boinc_get_init_data(m_UserData);
}

void BOINCClientAdapter::updateSharedMemoryArea()
{
	if(m_SharedMemoryArea) {

	  if(3 != sscanf( (char*)shmem,
			  		" <graphics_info> \n"
			  		" <skypos_rac> %f </skypos_rac> \n"
			  		" <skypos_dec> %f </skypos_dec> \n"
			  		" <fraction_done> %f </fraction_done> \n",
			  		&m_SkyPosRightAscension,
			  		&m_SkyPosDeclination,
			  		&m_WUFractionDone))
	  {		 
		  cerr << "Incompatible shared memory data encountered!" << endl;
	  }
	}
}

string BOINCClientAdapter::getCoreVersion() const
{
	// int major_version;int minor_version;int release;
}

string BOINCClientAdapter::getApplicationName() const
{
	
}

string BOINCClientAdapter::getApplicationVersion() const
{
	
}

string BOINCClientAdapter::getUserName() const
{
	
}

string BOINCClientAdapter::getTeamName() const
{
	
}

double BOINCClientAdapter::getUserCredit() const
{
	
}

double BOINCClientAdapter::getUserRACredit() const
{
	
}

double BOINCClientAdapter::getHostCredit() const
{
	
}

double BOINCClientAdapter::getHostRACredit() const
{
	
}

string BOINCClientAdapter::getWUName() const
{
	
}

double BOINCClientAdapter::getWUCPUTime() const
{
	
}

double BOINCClientAdapter::getWUFPOpsEstimated() const
{
	
}


double BOINCClientAdapter::getWUFPOpsBound() const
{
	
}


double BOINCClientAdapter::getWUMemoryBound() const
{
	
}

double BOINCClientAdapter::getWUDiskBound() const
{
	
}
