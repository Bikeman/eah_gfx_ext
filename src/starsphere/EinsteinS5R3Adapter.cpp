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

#include "EinsteinS5R3Adapter.h"
#include <cstdio>
#include <stdint.h>
#include <dirent.h>
#include "filesys.h" 


const string EinsteinS5R3Adapter::SharedMemoryIdentifier = "EinsteinHS";

const long EinsteinS5R3Adapter::MAX_RESULT_COUNT = 10000;


EinsteinS5R3Adapter::EinsteinS5R3Adapter(BOINCClientAdapter *boincClient)
{
	this->boincClient = boincClient;

	m_WUSkyPosRightAscension = 0.0;
	m_WUSkyPosDeclination = 0.0;
	m_last_RA = 0.0;
	m_last_dec = 0.0;
	m_WUFractionDone = 0.0;
	m_WUCPUTime = 0.0;
	m_last_WUCPUTime = 0.0;
	m_Nresults= 0;
	m_results = new EinsteinS5R3Result[MAX_RESULT_COUNT];
}

EinsteinS5R3Adapter::~EinsteinS5R3Adapter()
{
	delete [] m_results;
}

void EinsteinS5R3Adapter::refresh()
{

	boincClient->refresh();
	parseApplicationInformation();

	// check that some time has past since last checkpoint file loading	
	// if data is not loaded yet or marker position has changed
	// try to load checkpoint_file
	if( m_Nresults == 0 ||
		  (m_WUCPUTime - m_last_WUCPUTime) >  60.0  &&  
                    (m_last_RA != wuSkyPosRightAscension() || m_last_dec != wuSkyPosDeclination())) { 
			loadCheckpointFile();
			m_last_WUCPUTime = m_WUCPUTime;
			m_last_RA = wuSkyPosRightAscension();
			m_last_dec = wuSkyPosDeclination();
	}
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
		else {
			// convert radians to degrees
			m_WUSkyPosRightAscension *= 180/PI;
			m_WUSkyPosDeclination *= 180/PI;
		}
	}
}

void EinsteinS5R3Adapter::loadCheckpointFile() {
	UINT4 counter;
	int res;
	string fname("");
	// look for *.cpt file

	// directory traversal, not entirely portable, tho.
	DIR *pdir;
 	struct dirent *pent;

 	pdir=opendir("."); //"." refers to the current dir
	if (!pdir){
 		cerr << "opendir() failure;" << endl;
		closedir(pdir);
		return;
 	}

 	while ((pent=readdir(pdir))){
		fname =  pent->d_name;
		
		string::size_type pos1 = fname.find("h1_");
		string::size_type pos2 = fname.rfind(".cpt");

		if(pos1 == 0 && pos2 == fname.length() -4) {			
			break;
		} 
 	}
 	closedir(pdir);
	

	if(fname != "") {
		boinc_copy(fname.c_str(),"temp.cpt");
	}
	// try copying checkpoint file to tmp file
	

	// open tmp file and read candidates
	// plus normalize data 	
	// and set the current buffer and result nr..
	res=read_hfs_checkpoint("temp.cpt", &counter);
	// no error handling, we can't do anything about it anyways
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

long EinsteinS5R3Adapter::copyCandidates(float res [][3] , long n) const 
{
	long nr = m_Nresults;
 
	if(n < m_Nresults) {
	     nr = n;				
	}

	for (int i=0 ;  i < nr ; i++) {
		res[i][0]= m_results[i].ra;
		res[i][1]= m_results[i].dec;
		res[i][2]= m_results[i].meansig;
	}
	return nr;
}



int EinsteinS5R3Adapter::read_hfs_checkpoint(const char*filename, UINT4*counter) {
  FILE*fp;
  UINT4 len;
  UINT4 checksum;
  UINT4 tl_elems;
  HoughFStatOutputEntry buffer[MAX_RESULT_COUNT];  
  /* counter should be 0 if we couldn't read a checkpoint */
  *counter = 0;

  /* try to open file */
  fp = fopen(filename, "rb");
  if(!fp) {
      cerr << "Checkpoint "<< filename << " couldn't be opened\n";
      return(-1);
  }

  /* read number of elements */
  len = fread(&(tl_elems), sizeof(tl_elems), 1, fp);
  

  if(len != 1) {
    cerr << "Couldn't read elems from " <<  filename << endl;
    cerr << "fread() returned " << len << " , length was 1\n";
    if(fclose(fp))
      cerr << "In addition: couldn't close "<< filename << endl;
    return(-1);
  }
  /* sanity check */
  if (tl_elems > MAX_RESULT_COUNT) {
    cerr << "Number of elements read larger than max length of checkpoint data: " << tl_elems  << " > " << MAX_RESULT_COUNT <<"\n";
    if(fclose(fp))
      cerr << "In addition: couldn't close\n";
    return(-2);
  }

  /* read data */
  len = fread(buffer, sizeof(HoughFStatOutputEntry), tl_elems, fp);
  if(len != tl_elems) {
    cerr << "Couldn't read data from " << filename << endl;
    if(fclose(fp))
      cerr << "In addition: couldn't close " << filename << endl;
    return(-1);
  }

  /* read counter */
  len = fread(counter, sizeof(*counter), 1, fp);
  if(len != 1) {
    cerr << "Couldn't read counter from " << filename << endl;
    cerr << "fread() returned "<< len << ", length was 1\n";
    if(fclose(fp))
      cerr << "In addition: couldn't close " <<  filename << endl;
    return(-1);
  }

  /* read checksum */
  len = fread(&checksum, sizeof(checksum), 1, fp);
  if(len != 1) {
    cerr << "Couldn't read checksum to " <<  filename << endl;
    cerr << "fread() returned "<< len << ", length was 1\n";
    if(fclose(fp))
      cerr << "In addition: couldn't close "<< filename << endl;
    return(-1);
  }

  /* close file */
  if(fclose(fp)) {
    cerr << "Couldn't close " <<  filename << endl;
    return(-1);
  }

  /* verify checksum */
  for(len = 0; len < sizeof(tl_elems); len++)
    checksum -= *(((char*)&(tl_elems)) + len);
  for(len = 0; len < (tl_elems * sizeof(HoughFStatOutputEntry)); len++)
    checksum -= *(((char*)buffer) + len);
  for(len = 0; len < sizeof(*counter); len++)
    checksum -= *(((char*)counter) + len);
  if(checksum) {
    cerr << "Checksum error: "<< checksum << endl;
    return(-2);
  }


  for(UINT4 i=0 ; i < tl_elems ; i++) {
	m_results[i].ra = buffer[i].Alpha / PI * 180.0;
	m_results[i].dec= buffer[i].Delta / PI * 180.0;
	m_results[i].maxsig = buffer[i].HoughFStat;
	m_results[i].meansig = (buffer[i].MeanSig) > 0.0 ? buffer[i].MeanSig : 0.0 ; 
  }
  m_Nresults= tl_elems;		
  /* all went well */
  return(0);
}

