/***************************************************************************
 *   Copyright (C) 2008 by Oliver Bock                                     *
 *   oliver.bock[AT]aei.mpg.de                                             *
 *                                                                         *
 *   This file is part of Einstein@Home.                                   *
 *                                                                         *
 *   Einstein@Home is free software: you can redistribute it and/or modify *
 *   it under the terms of the GNU General Public License as published     *
 *   by the Free Software Foundation, version 2 of the License.            *
 *                                                                         *
 *   Einstein@Home is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 ***************************************************************************/

#include "BOINCClientAdapter.h"
#include "Libxml2Adapter.h"

#include <sstream>

BOINCClientAdapter::BOINCClientAdapter(string sharedMemoryIdentifier)
{
	m_Initialized = false;
	m_SharedMemoryAreaIdentifier = sharedMemoryIdentifier;
	m_SharedMemoryAreaAvailable = false;

	m_xmlIFace = new Libxml2Adapter();

	m_GraphicsFrameRate = 20;
	m_GraphicsQualitySetting = BOINCClientAdapter::LowGraphicsQualitySetting;
	m_GraphicsWindowWidth = 800;
	m_GraphicsWindowHeight = 600;
}

BOINCClientAdapter::~BOINCClientAdapter()
{
	if(m_xmlIFace) delete m_xmlIFace;
}

void BOINCClientAdapter::initialize()
{
	if(!m_Initialized) {
		readUserInfo();
		readSharedMemoryArea();
		readProjectPreferences();

		m_Initialized = true;
	}
}

void BOINCClientAdapter::refresh()
{
	if(m_Initialized) {
		readUserInfo();
		readSharedMemoryArea();

		/// \todo Check that we're still watching our own WU (or science app)!
	}
	else {
		cerr << "The BOINC Client Adapter has not yet been initialized! Doing so now..." << endl;
		initialize();
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

void BOINCClientAdapter::readProjectPreferences()
{
	string temp;
	istringstream convertor;
	convertor.exceptions(ios_base::badbit | ios_base::failbit);

	// prepare xml document
	m_xmlIFace->setXmlDocument(projectInformation(), "http://einstein.phys.uwm.edu");

	// use XPath queries to get attributes
	try {
		temp = m_xmlIFace->getSingleNodeContentByXPath("/project_preferences/graphics/@fps");
		if(temp.length() > 0) {
			convertor.str(temp);
			convertor >> dec >> m_GraphicsFrameRate;
			convertor.clear();
		}

		temp = m_xmlIFace->getSingleNodeContentByXPath("/project_preferences/graphics/@quality");
		if(temp.length() > 0) {
			if(temp == "high") {
				m_GraphicsQualitySetting = BOINCClientAdapter::HighGraphicsQualitySetting;
			}
			else if(temp == "medium") {
				m_GraphicsQualitySetting = BOINCClientAdapter::MediumGraphicsQualitySetting;
			}
			else {
				m_GraphicsQualitySetting = BOINCClientAdapter::LowGraphicsQualitySetting;
			}
		}

		temp = m_xmlIFace->getSingleNodeContentByXPath("/project_preferences/graphics/@width");
		if(temp.length() > 0) {
			convertor.str(temp);
			convertor >> dec >> m_GraphicsWindowWidth;
			convertor.clear();
		}

		temp = m_xmlIFace->getSingleNodeContentByXPath("/project_preferences/graphics/@height");
		if(temp.length() > 0) {
			convertor.str(temp);
			convertor >> dec >> m_GraphicsWindowHeight;
			convertor.clear();
		}
	}
	catch(ios_base::failure) {
		cerr << "Error parsing project preferences! Using defaults..." << endl;
	}
}

string BOINCClientAdapter::applicationInformation() const
{
	return m_SharedMemoryAreaContents;
}

string BOINCClientAdapter::projectInformation() const
{
	// ugly workaround for incomplete XML fragment returned by BOINC!
	string temp = "<project_preferences>\n";
	temp += m_UserData.project_preferences;
	temp += "</project_preferences>\n";

	return temp;
}

int BOINCClientAdapter::graphicsFrameRate() const
{
	return m_GraphicsFrameRate;
}

BOINCClientAdapter::GraphicsQualitySetting BOINCClientAdapter::graphicsQualitySetting() const
{
	return m_GraphicsQualitySetting;
}

int BOINCClientAdapter::graphicsWindowWidth() const
{
	return m_GraphicsWindowWidth;
}

int BOINCClientAdapter::graphicsWindowHeight() const
{
	return m_GraphicsWindowHeight;
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

double BOINCClientAdapter::wuCPUTimeSpent() const
{
	return m_UserData.wu_cpu_time;
}
