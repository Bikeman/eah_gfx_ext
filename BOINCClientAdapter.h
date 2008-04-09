#ifndef BOINCCLIENTADAPTER_H_
#define BOINCCLIENTADAPTER_H_

#include <iostream>
#include <string>
#include <sstream>

#include "boinc_api.h"
#include "graphics2.h"

using namespace std;

/**
 * This is the common name tag used to identify the shared memory area
 * provided by the \b Einstein\@Home application
 */
#define EAH_SHMEM_APP_NAME "EinsteinHS"

/**
 * \brief Adapter class which facilitates all communications with the BOINC client
 * 
 * This adapter class can be used to query the BOINC core client as well as the \b Einstein\@Home
 * application for informational data.
 * 
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class BOINCClientAdapter
{
public:
	/// Default constructor
	BOINCClientAdapter();
	
	/// Destructor
	virtual ~BOINCClientAdapter();
	
	/**
	 * \brief Refreshes dynamic data (e.g. search information)
	 * 
	 * You want to call this method periodically to refresh any volatile client information
	 * 	
	 * \see AbstractGraphicsEngine::refreshBOINCInformation()
	 */
	void refresh();
	
	/**
	 * \brief Retrieves the BOINC core client version of the currently active client
	 * 
	 * \return The BOINC core client version
	 */
	string coreVersion() const;
	
	/**
	 * \brief Retrieves the \b Einstein\@Home application name of the currently active work unit
	 * 
	 * \return The \b Einstein\@Home application name
	 */
	string applicationName() const;
	
	/**
	 * \brief Retrieves the \b Einstein\@Home application version of the currently active work unit
	 * 
	 * \return The \b Einstein\@Home application version
	 */
	string applicationVersion() const;
	
	/**
	 * \brief Retrieves the BOINC user name currently logged in
	 * 
	 * \return The BOINC user name
	 */
	string userName() const;
	

	/**
	 * \brief Retrieves the BOINC team name of the currently logged in user
	 * 
	 * \return The BOINC team name
	 */
	string teamName() const;
	
	/**
	 * \brief Retrieves the total project credit of the currently logged in user
	 * 
	 * \return The total project credit
	 */	
	double userCredit() const;
	
	/**
	 * \brief Retrieves the recent average project credit (RAC) of the currently logged in user
	 * 
	 * \return The recent average project credit
	 */	
	double userRACredit() const;
	
	/**
	 * \brief Retrieves the total project credit of this host
	 * 
	 * \return The total project credit
	 */	
	double hostCredit() const;
	
	/**
	 * \brief Retrieves the recent average project credit (RAC) of this host
	 * 
	 * \return The recent average project credit
	 */	
	double hostRACredit() const;

	/**
	 * \brief Retrieves the name of the currently active work unit
	 * 
	 * \return The work unit name
	 */
	string wuName() const;
	
    double wuFPOpsEstimated() const;
    double wuFPOpsBound() const;
    double wuMemoryBound() const;
    double wuDiskBound() const;

    /**
	 * \brief Retrieves the right ascension of the currently searched sky position
	 * 
	 * \return The right ascension (in radians)
	 */
    double wuSkyPosRightAscension() const;
    
    /**
	 * \brief Retrieves the declination of the currently searched sky position
	 * 
	 * \return The right ascension (in radians)
	 */
    double wuSkyPosDeclination() const;
    
    /**
     * \brief Retrieves the completion fraction of the currently active work unit
     * 
     * \return The completion fraction (range 0-1)
     */    
    double wuFractionDone() const;
    
    /**
     * \brief Retrieves the amount of CPU time consumed for the currently active work unit
     * during the active session
     * 
     * \return The accumulated CPU time consumed during this work unit session (in seconds)
     */
    double wuCPUTime() const;
	
private:
    /**
     * \brief Fetch the contents of \c init_data.xml
     * 
     * This method uses the BOINC API in order to fill the \c APP_INIT_DATA structure m_UserData
     * with initial information about the current work unit computation session (slot). The data
     * in \c init_data.xml is refreshed only at the beginning of a session, hence this method doesn't
     * need to be called periodically.
     */
	void readUserInfo();
	
	/**
	 * \brief Fetch the contents of the shared memory area provided by the \b Einstein\@Home application
	 * 
	 * The shared memory area contains various informational bits and pieces about the running application
	 * and work unit computation. The contents have to be considered as volatile, hence should be refreshed
	 * periodically.
	 * 	 
	 * \see refresh()
	 */
	void readSharedMemoryArea();
	
	/// Pointer to the shared memory area
	char *m_SharedMemoryArea;
	
	/// Flag to indicate whether the shared memory area is available or not
	bool m_SharedMemoryAreaAvailable;
	
	/**
	 * \brief Information structure returned by the BOINC client API.
	 * 
	 * It contains initial information about the current work unit computation session.
	 */
	APP_INIT_DATA m_UserData;
	
	/// Right ascension of the currently searched sky position
	double m_WUSkyPosRightAscension;
	
	/// Declination of the currently searched sky position
	double m_WUSkyPosDeclination;
	
	/// The completion fraction of the active work unit
	double m_WUFractionDone;
	
	/// Amount of CPU time consumed for the work unit during the active session
	double m_WUCPUTime;
};

#endif /*BOINCCLIENTADAPTER_H_*/
