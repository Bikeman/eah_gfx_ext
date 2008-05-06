#ifndef STARSPHERES5R3_H_
#define STARSPHERES5R3_H_

#include <sstream>

#include "Starsphere.h"
#include "EinsteinS5R3Adapter.h"

using namespace std;

/**
 * \brief Specialized rendering engine for the S5R3 science run
 * 
 * This class comprises the specialized parts of the Starsphere rendering engine.
 * The main differences stem from the fact that most science runs differ in their
 * search configuration and parameters. Thus the parameters exposed by the HUD
 * (head-up display) are positioned and rendered here. For the time being the
 * "BOINC Statistics" are top-aligned to the "Search Parameters", hence they're
 * also positioned and rendered here.
 * 
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class StarsphereS5R3 : public Starsphere
{
public:
	/// Default contructor
	StarsphereS5R3();
	
	/// Destructor
	virtual ~StarsphereS5R3();
	
	/**
	 * \brief This method is called to initialize the engine
	 * 
	 * As this method overrides its parent's implementation, it calls Starsphere::initialize()
	 * first in order to "add" the sepcialized parts afterwards.
	 * 
	 * \param width The current width of the display surface
	 * \param height The current height of the display surface
	 * \param font A pointer to a Resource object containing TTF font faces for text rendering 
	 */
	void initialize(const int width, const int height, const Resource *font);
	
	/**
	 * \brief This method is called when the windowing system encounters a window resize event
	 * 
	 * As this method overrides its parent's implementation, it calls Starsphere::resize()
	 * first in order to "add" the sepcialized parts afterwards.
	 * 
	 * \param width The new width of the display surface
	 * \param height The new height of the display surface
	 */
	void resize(const int width, const int height);
	
	/**
	 * \brief This method is called when the BOINC client information should be updated
	 * 
	 * This method implements AbstractGraphicsEngine::refreshBOINCInformation() and calls
	 * Starsphere::refreshLocalBOINCInformation() first and "adds" the sepcialized
	 * parts afterwards.
	 * 
	 * \see AbstractGraphicsEngine::refreshBOINCInformation()
	 * \see Starsphere::refreshLocalBOINCInformation()
	 */
	void refreshBOINCInformation();
	
private:
	/**
	 * \brief Render science run specific search information
	 * 
	 * For this specific implementation this also includes the "BOINC Statistics"
	 * as it is top-aligned to the "Search Information".
	 */
	void renderSearchInformation();
	
	/// Specialized BOINC client adapter instance for information retrieval
	EinsteinS5R3Adapter m_EinsteinAdapter;
	
	/// Local copy of the current WU's search parameter "Right-Ascension"
	string m_WUSkyPosRightAscension;
	
	/// Local copy of the current WU's search parameter "Declination"
	string m_WUSkyPosDeclination;
	
	/// Local copy of the current WU's search parameter "Percent done"
	string m_WUPercentDone;
	
	/// Local copy of the current WU's search parameter "CPU Time"
	string m_WUCPUTime;
	
	/// HUD configuration setting (line offset for medium sized font)
	GLfloat m_YOffsetMedium;
	
	/// HUD configuration setting (horizontal start position for the right part)
	GLfloat m_XStartPosRight;
	
	/// HUD configuration setting (vertical start postion for the bottom part, header)
	GLfloat m_YStartPosBottom;
	
	/// HUD configuration setting (vertical start postion for the bottom part, line 1)
	GLfloat m_Y1StartPosBottom;
	
	/// HUD configuration setting (vertical start postion for the bottom part, line 2)
	GLfloat m_Y2StartPosBottom;
	
	/// HUD configuration setting (vertical start postion for the bottom part, line 3)
	GLfloat m_Y3StartPosBottom;
	
	/// HUD configuration setting (vertical start postion for the bottom part, line 4)
	GLfloat m_Y4StartPosBottom;
};

#endif /*STARSPHERES5R3_H_*/
