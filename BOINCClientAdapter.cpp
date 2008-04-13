#include "BOINCClientAdapter.h"

BOINCClientAdapter::BOINCClientAdapter()
{
	m_Initialized = false;
	m_SharedMemoryAreaAvailable = false;
}

BOINCClientAdapter::~BOINCClientAdapter()
{
}

void BOINCClientAdapter::initialize(string sharedMemoryIdentifier)
{
	m_SharedMemoryAreaIdentifier = sharedMemoryIdentifier;
	
	readUserInfo();
	readSharedMemoryArea();
	
	m_Initialized = true;
}

void BOINCClientAdapter::refresh()
{
	if(m_Initialized) {
		readSharedMemoryArea();
	}
	else {
		cerr << "The BOINC Client Adapter has not yet been initialized!";
	}
}

void BOINCClientAdapter::readUserInfo()
{
	boinc_parse_init_data_file();
	boinc_get_init_data(m_UserData);
}

void BOINCClientAdapter::readSharedMemoryArea()
{
	// check if we already have a pointer
	if(m_SharedMemoryAreaAvailable) {
		// load contents
		m_SharedMemoryAreaContents = string(m_SharedMemoryArea);
	}
	// the shared memory area's not available, try to get a pointer to it
	else {
	   m_SharedMemoryArea = (char*) boinc_graphics_get_shmem((char*)m_SharedMemoryAreaIdentifier.c_str());
	    
	    if(m_SharedMemoryArea) {
	    	// fine, get the contents recursively
	        m_SharedMemoryAreaAvailable = true;
	        readSharedMemoryArea();	        
	    }
	    else {
	    	// bad luck
	    	m_SharedMemoryAreaContents = "";
	        m_SharedMemoryAreaAvailable = false;
	    }
	}
}

string BOINCClientAdapter::applicationInformation() const
{
	return m_SharedMemoryAreaContents;
}

string BOINCClientAdapter::coreVersion() const
{
	stringstream buffer;
	
	// build common version string
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
