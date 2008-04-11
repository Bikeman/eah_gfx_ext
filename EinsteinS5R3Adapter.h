#ifndef EINSTEINS5R3ADAPTER_H_
#define EINSTEINS5R3ADAPTER_H_

#include <string>

#include "BOINCClientAdapter.h"

using namespace std;

/**
 * \brief Adapter class which facilitates communications with the \b Einstein\@Home S5R3 application
 * 
 * This adapter class can be used to query the \b Einstein\@Home S5R3 application
 * for informational data about the current work unit like search details and progress.
 * 
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class EinsteinS5R3Adapter
{
public:
	/**
	 * Constructor
	 * 
	 * \param boincClient Pointer to the general BOINC client adapter instance
	 */
	EinsteinS5R3Adapter(BOINCClientAdapter* boincClient);
	
	/// Destructor
	virtual ~EinsteinS5R3Adapter();
	
	/**
	 * \brief Refreshes dynamic data (e.g. search information)
	 * 
	 * You want to call this method periodically to refresh any volatile application information
	 * 	
	 * \see AbstractGraphicsEngine::refreshBOINCInformation
	 */
	void refresh();
	
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
	void parseApplicationInformation();
	
	BOINCClientAdapter *boincClient;
	
	/// Right ascension of the currently searched sky position
	double m_WUSkyPosRightAscension;
	
	/// Declination of the currently searched sky position
	double m_WUSkyPosDeclination;
	
	/// The completion fraction of the active work unit
	double m_WUFractionDone;
	
	/// Amount of CPU time consumed for the work unit during the active session
	double m_WUCPUTime;	
};

#endif /*EINSTEINS5R3ADAPTER_H_*/
