#include "BOINCClientAdapter.h"

BOINCClientAdapter::BOINCClientAdapter()
{
	m_WUSkyPosRightAscension = 0.0;
	m_WUSkyPosDeclination = 0.0;
	m_WUFractionDone = 0.0;
	m_WUCPUTime = 0.0;
	
    m_SharedMemoryAreaAvailable = false;
    
	readUserInfo();
	readSharedMemoryArea();
}

BOINCClientAdapter::~BOINCClientAdapter()
{
}

void BOINCClientAdapter::refresh()
{
	readSharedMemoryArea();
}

void BOINCClientAdapter::readUserInfo()
{
	boinc_parse_init_data_file();
	boinc_get_init_data(m_UserData);
}

/**
 * \todo This should be more flexible as the shared memory area's
 * contents are very likely to change over time, i.e. when a new
 * application (and/or search method) is implemented!
 * \note We might use the factory method pattern here
 */
void BOINCClientAdapter::readSharedMemoryArea()
{
	// check if we already have a pointer
	if(m_SharedMemoryAreaAvailable) {

		// parse the current contents (see notes above!)
		if(4 != sscanf(m_SharedMemoryArea,
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
	// the shared memory area's not available, try to get a pointer to it
	else {
	   m_SharedMemoryArea = (char*) boinc_graphics_get_shmem(EAH_SHMEM_APP_NAME);
	    
	    if(m_SharedMemoryArea) {
	    	// fine, get the contents recursively
	        m_SharedMemoryAreaAvailable = true;
	        readSharedMemoryArea();	        
	    }
	    else {
	    	// bad luck
	        m_SharedMemoryAreaAvailable = false;
	    }
	}
}

string BOINCClientAdapter::coreVersion() const
{
	stringstream buffer;
	buffer	<< m_UserData.major_version << "."
			<< m_UserData.minor_version << "."
			<< m_UserData.release;
	
	return string(buffer.str());
}

string BOINCClientAdapter::applicationName() const
{
	return string(m_UserData.app_name);
}

string BOINCClientAdapter::applicationVersion() const
{
	stringstream buffer;
	buffer << m_UserData.app_version;
	
	return string(buffer.str());	
}

string BOINCClientAdapter::userName() const
{
	return string(m_UserData.user_name);
}

string BOINCClientAdapter::teamName() const
{
	return string(m_UserData.team_name);
}

double BOINCClientAdapter::userCredit() const
{
	return m_UserData.user_total_credit;
}

double BOINCClientAdapter::userRACredit() const
{
	return m_UserData.user_expavg_credit;
}

double BOINCClientAdapter::hostCredit() const
{
	return m_UserData.host_total_credit;
}

double BOINCClientAdapter::hostRACredit() const
{
	return m_UserData.host_expavg_credit;	
}

string BOINCClientAdapter::wuName() const
{
	return string(m_UserData.wu_name);	
}

double BOINCClientAdapter::wuFPOpsEstimated() const
{
	return m_UserData.rsc_fpops_est;	
}


double BOINCClientAdapter::wuFPOpsBound() const
{
	return m_UserData.rsc_fpops_bound;
}


double BOINCClientAdapter::wuMemoryBound() const
{
	return m_UserData.rsc_memory_bound;
}

double BOINCClientAdapter::wuDiskBound() const
{
	return m_UserData.rsc_disk_bound;
}

double BOINCClientAdapter::wuSkyPosRightAscension() const
{
	return m_WUSkyPosRightAscension;
}

double BOINCClientAdapter::wuSkyPosDeclination() const
{
	return m_WUSkyPosDeclination;
}

double BOINCClientAdapter::wuFractionDone() const
{
	return m_WUFractionDone;
}

double BOINCClientAdapter::wuCPUTime() const
{
	return m_WUCPUTime;
}
