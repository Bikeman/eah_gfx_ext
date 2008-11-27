/***************************************************************************
 *   Copyright (C) 2004 David Hammer, Eric Myers, Bruce Allen              *
 *   Copyright (C) 2008 Bernd Machenschalk                                 *
 *                                                                         *
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

#ifndef STARSPHERE_H_
#define STARSPHERE_H_

#include <cmath>
#include <cstdio>
#include <ctime>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#include <SDL.h>
#include <SDL_opengl.h>

#include <oglft/OGLFT.h>

#include <util.h>

#include "AbstractGraphicsEngine.h"
#include "EinsteinS5R3Adapter.h"

// SIN and COS take arguments in DEGREES
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

// search marker status
#define MARKER_NONE 0
#define MARKER_SHOW 1
#define MARKER_NEW  2

// needed to find OpenGL 1.4 prototypes in glext.h (alternatives?)
#define GL_GLEXT_PROTOTYPES

using namespace std;

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief %Starsphere rendering engine for \b Einstein\@Home
 *
 * This class comprises the generic parts of the %Starsphere rendering engine.
 * %Starsphere displays the celestial sphere indicating a fixed set of stars with
 * their constellations as well as known pulsars and supernova remnants. Apart from
 * that the four main gravitational wave observatory locations are shown at their actual
 * real-time position. Furthermore a head-up display (HUD) shows relevant BOINC
 * statistics as well as information on the current workunit (WU) being processed.
 *
 * For more details please refer to http://einstein.phys.uwm.edu/starsphere.php
 *
 * Note: all science run specific parts are implemented in specialized subclasses
 * of this engine.
 *
 * \todo The code of this implementaion is based on the former version of %Starsphere
 * and there's still some refactoring, code cleanup and documenting left to be done.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class Starsphere : public AbstractGraphicsEngine
{
public:
	/// Destructor
	virtual ~Starsphere();

	/**
	 * \brief This method is called to initialize the engine
	 *
	 * \param width The current width of the display surface
	 * \param height The current height of the display surface
	 * \param font A pointer to a Resource object containing TTF font faces for text rendering
	 * \param recycle This flag indicates whether we initialize (FALSE) or reinitialize (TRUE) the context
	 */
	virtual void initialize(const int width, const int height, const Resource *font, const bool recycle = false);

	/**
	 * \brief This method is called when the windowing system encounters a window resize event
	 *
	 * \param width The new width of the display surface
	 * \param height The new height of the display surface
	 */
	virtual void resize(const int width, const int height);

	/**
	 * \brief This method renders one frame of the animation
	 *
	 * \param timeOfDay The current time (e.g. BOINC's dtime(), used for time-based rendering evolution)
	 */
	void render(const double timeOfDay);

	/**
	 * \brief Event handler for mouse button events
	 *
	 * \param positionX The mouse position's x-coordinate
	 * \param positionY The mouse position's y-coordinate
	 * \param buttonPressed The mouse button pressed
	 */
	void mouseButtonEvent(const int positionX, const int positionY,
						  const AbstractGraphicsEngine::MouseButton buttonPressed);

	/**
	 * \brief Event handler for mouse move events
	 *
	 * \param deltaX The relative mouse movement in the x-direction
	 * \param deltaY The relative mouse movement in the y-direction
	 * \param buttonPressed The mouse button pressed
	 */
	void mouseMoveEvent(const int deltaX, const int deltaY,
						const AbstractGraphicsEngine::MouseButton buttonPressed);

	/**
	 * \brief Event handler for key press events
	 *
	 * \param keyPressed The key pressed
	 */
	void keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed);

protected:
	/**
	 * \brief Constructor
	 *
	 * The constructor is protected since this is an abstract class. It takes
	 * as an argument the name of the shared memory area which is propagated
	 * to the BOINC client adapter instance (during construction).
	 *
	 * \param sharedMemoryIdentifier The identifier of the shared memory area
	 *
	 * \see AbstractGraphicsEngine::AbstractGraphicsEngine()
	 * \see BOINCClientAdapter::BOINCClientAdapter()
	 */
	Starsphere(string sharedMemoryIdentifier);

	/**
	 * \brief Render science run specific logo
	 *
	 * This abtract method is to be defined by derived classes implementing
	 * the science run specific logo rendering.
	 */
	inline virtual void renderLogo() = 0;

	/**
	 * \brief Render science run specific search information
	 *
	 * This abtract method is to be defined by derived classes implementing
	 * the science run specific search information handling and rendering.
	 *
	 * Note: for this engine this also includes the "BOINC Statistics"
	 * as it is top-aligned to the "Search Information".
	 */
	inline virtual void renderSearchInformation() = 0;

	/**
	 * \brief Render additional observatories
	 *
	 * This method doesn't do anything in its local implementation. It's provided
	 * for potential specializing classes to add additional observatories by
	 * overriding the empty default implementation.
	 *
	 * Important: overriding classes should just provide additional display lists
	 * by via calls to glCallList().
	 *
	 * \see StarsphereRadio::renderAdditionalObservatories()
	 */
	inline virtual void renderAdditionalObservatories();

	/**
	 * \brief This method has to be called in order to update the BOINC client information
	 *
	 * This is the local/generic implementation which calls
	 * AbstractGraphicsEngine::refreshLocalBOINCInformation() first and
	 * refreshes the "BOINC Statistics" afterwards.
	 *
	 * \see AbstractGraphicsEngine::refreshLocalBOINCInformation()
	 */
	virtual void refreshLocalBOINCInformation();

	/**
	 * \brief Generates the OpenGL call lists for the displayed observatories
	 *
	 * \param dimFactor A dim factor (range: 0 <= x <= 1) that will, well, dim the color
	 * of the observatories.
	 */
	virtual void generateObservatories(const float dimFactor);

	/**
	 * \brief Available feature IDs
	 *
	 * \see Starsphere::setFeature()
	 * \see Starsphere::isFeature()
	 */
	enum Features {
		STARS = 1,
		CONSTELLATIONS = 2,
		OBSERVATORIES = 4,
		XRAYS = 8,
		PULSARS = 16,
		SNRS = 32,
		GLOBE = 64,
		AXES = 128,
		SEARCHINFO = 256,
		LOGO = 512,
		MARKER = 1024
	};

	/**
	 * \brief Set the display state of a certain feature
	 *
	 * \param feature The feature to enable/disable
	 * \param enable The state to set for the feature
	 *
	 * \see Starsphere::Features
	 * \see Starsphere::isFeature()
	 */
	void setFeature(const Features feature, const bool enable);

	/**
	 * \brief Query the display state of a certain feature
	 *
	 * \param feature The feature to query
	 *
	 * \return The current state of the feature
	 *
	 * \see Starsphere::Features
	 * \see Starsphere::setFeature()
	 */
	inline bool isFeature(const Features feature);

	/**
	 * \brief Computes the Right Ascension of the zenith at a given time (from
	 * the Unix epoch, in seconds) at a given Longitude (in degrees)
	 *
	 * From 'The Cambridge Handbook of Physics Formulas', Graham Woan, 2003
	 * edition, CUP. (NOT the first edition), p177.
	 *
	 * \param T Current time in seconds since the epoch
	 * \param LONdeg Longitude in degrees
	 *
	 * \return The right ascension of the zenith
	 */
	GLfloat RAofZenith(double T, GLfloat LONdeg);

	/**
	 * \brief Creates a GL vertex in 3D sky sphere coordinates
	 *
	 * Use like glVertex()
	 *
	 * \param RAdeg The right ascension of the new vertex
	 * \param DEdeg The declination of the new vertex
	 * \param radius The radius of the sky sphere
	 */
	void sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius);

	/**
	 * \brief Creates a GL vertex on the surface of the sky sphere.
	 *
	 * Use like glVertex()
	 *
	 * \param RAdeg The right ascension of the new vertex
	 * \param DEdeg The declination of the new vertex
	 */
	void sphVertex(GLfloat RAdeg, GLfloat DEdeg);

	/// Radius of the celestial sphere
	GLfloat sphRadius;

	/// Observatory movement (in seconds since 1970 with usec precision)
	double m_ObservatoryDrawTimeLocal;

	// resource handling

	/// Font resource instance
	const Resource *m_FontResource;

	/// Font texture instance for logo title rendering
	OGLFT::TranslucentTexture *m_FontLogo1;

	/// Font texture instance for logo subtitle rendering
	OGLFT::TranslucentTexture *m_FontLogo2;

	/// Font texture instance for info box header rendering
	OGLFT::TranslucentTexture *m_FontHeader;

	/// Font texture instance for info box content rendering
	OGLFT::TranslucentTexture *m_FontText;

	// Graphics state info:

	/// Current window width (x-resolution)
	int m_CurrentWidth;

	/// Current window height (y-resolution)
	int m_CurrentHeight;

	/// Current window aspect ration
	float aspect;


	// HUD text rendering config (maybe overridden in subclasses)

	/// X-coordinate position for head up display (HUD) positioning
	GLfloat m_XStartPosLeft;

	/// Y-coordinate position for head up display (HUD) positioning
	GLfloat m_YStartPosTop;

	/// Y-coordinate line offset for head up display (HUD) positioning
	GLfloat m_YOffsetLarge;

	// local HUD contents

	/// User name to be displayed in "BOINC Information" panel
	string m_UserName;

	/// Team name to be displayed in "BOINC Information" panel
	string m_TeamName;

	/// User total credit to be displayed in "BOINC Information" panel
	string m_UserCredit;

	/// User recent average credit to be displayed in "BOINC Information" panel
	string m_UserRACredit;

	// search marker info

	/// Current right ascension of the search marker (gunsight)
	double m_CurrentRightAscension;

	/// Current declination of the search marker (gunsight)
	double m_CurrentDeclination;

	/// Refresh indicator when the search marker (gunsight) coordinates changed
	bool m_RefreshSearchMarker;

private:
	/// Generate OpenGL display list for stars
	void make_stars();

	/// Generate OpenGL display list for pulsars
	void make_pulsars();

	/// Generate OpenGL display list for SNRs
	void make_snrs();

	/// Generate OpenGL display list for constellations
	void make_constellations();

	/// Generate OpenGL display list for the axes (debug)
	void make_axes();

	/// Generate OpenGL display list for the globe
	void make_globe();

	/**
	 * \brief Generate OpenGL display list for search marker (gunsight)
	 *
	 * \param RAdeg Right ascension in degrees
	 * \param DEdeg Declination in degrees
	 * \param size Base size (radius/length) of the marker
	 */
	void make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size);

	/**
	 * \brief Generate a single star vertex
	 *
	 * \param RAdeg Right ascension in degrees
	 * \param DEdeg Declination in degrees
	 * \param size Point size of the star
	 */
	void star_marker(float RAdeg, float DEdeg, float size);


	/// Feature display list ID's
	GLuint Axes, Stars, Constellations, Pulsars, SNRs;

	/// Feature display list ID's
	GLuint LLOmarker, LHOmarker, GEOmarker, VIRGOmarker;

	/// Feature display list ID's
	GLuint sphGrid, SearchMarker;

	/**
	 * \brief Current state of all features (bitmask)
	 * \see Starsphere::Features()
	 * \see Starsphere::setFeature()
	 * \see Starsphere::isFeature()
	 */
	int featureFlags;

	// Viewpoint (can be changed with mouse)

	/// Viewpoint azimuth in degrees
	GLfloat viewpt_azimuth;

	/// Viewpoint elevation angle in degrees
	GLfloat viewpt_elev;

	/// Viewpoint distance out
	GLfloat viewpt_radius;

	/// Viewpoint wobble amplitude in degrees
	GLfloat wobble_amp;

	/// Viewpoint wobble up/down period in minutes
	GLfloat wobble_period;

	/// Viewpoint radial zoom amplitude
	GLfloat zoom_amp;

	/// Viewpoint zoom in/out period in minutes
	GLfloat zoom_period;

	/// Viewpoint rotation offset
	GLfloat rotation_offset;

	/// Viewpoint rotations in degrees per minute
	GLfloat rotation_speed;

	// view control

	/**
	 * \brief Rotates the sphere by changing the viewpoint rotation/elevation relatively
	 *
	 * \param relativeRotation Relative rotation factor (e.g. relative mouse movement)
	 * \param relativeElevation Relative elevation factor (e.g. relative mouse movement)
	 *
	 * \see Starsphere::mouseMoveEvent()
	 */
	void rotateSphere(const int relativeRotation, const int relativeElevation);

	/**
	 * \brief Zooms the sphere by changing the viewpoint radius relatively
	 *
	 * \param relativeZoom Relative zoom factor (e.g. relative mouse movement)
	 *
	 * \see Starsphere::mouseMoveEvent()
	 */
	void zoomSphere(const int relativeZoom);
};

/// Constellation & star coordinates (starlist.C)
extern float star_info[][2];

/// Total number of stars
extern int Nstars;

/// Pulsar coordinates are (pulsar_list.C)
extern float pulsar_info[][2];

/// Total number of pulsars
extern int Npulsars;

/// SNR coordinates (snr_list.C)
extern float SNR_info[][2];

/// Total number of SNRs
extern int NSNRs;

/**
 * @}
 */

#endif /*STARSPHERE_H_*/
