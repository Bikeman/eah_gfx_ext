#include "BOINCClientAdapter.h"

BOINCClientAdapter::BOINCClientAdapter()
{
    updateUserInfo();
    
    m_SharedMemoryAreaAvailable = false;
	readSharedMemoryArea();
}

BOINCClientAdapter::~BOINCClientAdapter()
{
}

void BOINCClientAdapter::readUserInfo()
{
	boinc_get_init_data(m_UserData);
}

void BOINCClientAdapter::readSharedMemoryArea()
{
	if(m_SharedMemoryAreaAvailable) {

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
	else {
	   m_SharedMemoryArea = (char*) boinc_graphics_get_shmem(EAH_SHMEM_APP_NAME);
	    
	    if(m_SharedMemoryArea) {
	        m_SharedMemoryAreaAvailable = true;
	        readSharedMemoryArea();	        
	    }
	    else {
	        m_SharedMemoryAreaAvailable = false;
	    }
	}
}

string BOINCClientAdapter::coreVersion() const
{
	// int major_version;int minor_version;int release;
}

string BOINCClientAdapter::applicationName() const
{
	
}

string BOINCClientAdapter::applicationVersion() const
{
	
}

string BOINCClientAdapter::userName() const
{
	
}

string BOINCClientAdapter::teamName() const
{
	
}

double BOINCClientAdapter::userCredit() const
{
	
}

double BOINCClientAdapter::userRACredit() const
{
	
}

double BOINCClientAdapter::hostCredit() const
{
	
}

double BOINCClientAdapter::hostRACredit() const
{
	
}

string BOINCClientAdapter::wuName() const
{
	
}

double BOINCClientAdapter::wuCPUTime() const
{
	
}

double BOINCClientAdapter::wuFPOpsEstimated() const
{
	
}


double BOINCClientAdapter::wuFPOpsBound() const
{
	
}


double BOINCClientAdapter::wuMemoryBound() const
{
	
}

double BOINCClientAdapter::wuDiskBound() const
{
	
}
