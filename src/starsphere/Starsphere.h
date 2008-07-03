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


/* SIN and COS take arguments in DEGREES */
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

/* search marker status */
#define MARKER_NONE 0
#define MARKER_SHOW 1
#define MARKER_NEW  2

// needed to find OpenGL 1.4 prototypes in glext.h (alternatives?)
#define GL_GLEXT_PROTOTYPES

using namespace std;


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
	void render(const double timeOfDay);

	// event handling
	void mouseButtonEvent(const int positionX, const int positionY,
						  const AbstractGraphicsEngine::MouseButton buttonPressed);
	void mouseMoveEvent(const int deltaX, const int deltaY,
						const AbstractGraphicsEngine::MouseButton buttonPressed);
	void keyboardPressEvent(const AbstractGraphicsEngine::KeyBoardKey keyPressed);
	
protected:
	/**
	 * \brief Default contructor
	 * 
	 * The constructor is protected since this an abstract class.
	 */	
	Starsphere();
	
	/**
	 * \brief Render science run specific search information
	 * 
	 * This abtract method is to be defined by derived classes implementing
	 * the science run specific search information handling and rendering.
	 * 
	 * Note: for this engine this also includes the "BOINC Statistics"
	 * as it is top-aligned to the "Search Information".
	 */
	virtual void renderSearchInformation() = 0;
	
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
	
	// resource handling
	const Resource *m_FontResource;
	OGLFT::TranslucentTexture *m_FontLogo1;
	OGLFT::TranslucentTexture *m_FontLogo2;
	OGLFT::TranslucentTexture *m_FontHeader;
	OGLFT::TranslucentTexture *m_FontText;
	
	// Graphics state info:
	int m_CurrentWidth;
	int m_CurrentHeight;
	float aspect;
	
	// HUD text rendering config (maybe overridden in subclasses)
	GLfloat m_XStartPosLeft;
	GLfloat m_YStartPosTop;
	GLfloat m_YOffsetLarge;
	
	// local HUD contents
	string m_UserName;
	string m_TeamName;
	string m_UserCredit;
	string m_UserRACredit;
	
	// search marker info
	double m_CurrentRightAscension;
	double m_CurrentDeclination;
	bool m_RefreshSearchMarker;
	
private:
	void make_stars();
	void make_pulsars();
	void make_snrs();
	void make_constellations();
	void make_obs();
	void make_axes();
	void make_globe();
	void make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size);

	GLfloat RAofZenith(double T, GLfloat LONdeg);
	void sphVertex3D(GLfloat RAdeg, GLfloat DEdeg, GLfloat radius);
	void sphVertex(GLfloat RAdeg, GLfloat DEdeg);
	void star_marker(float RAdeg, float DEdeg, float size);

	/**
	 * Object ID's  and on/off switches.
	 * (someday the keyboard can be used to turn display of items on/off)
	 */
	GLuint Axes, Stars, Constellations, Pulsars;
	GLuint LLOmarker, LHOmarker, GEOmarker, VIRGOmarker;
	GLuint sphGrid, SNRs, SearchMarker;

	/**
	 * Parameters and State info:
	 */
	GLfloat sphRadius;
	int featureFlags;

	/**
	 * Viewpoint (can be changed with mouse)
	 */
	GLfloat viewpt_azimuth; // azimuth, in degrees
	GLfloat viewpt_elev; // elevation angle, in degrees
	GLfloat viewpt_radius; // distance out

	GLfloat wobble_amp; // wobble amplitude, in degrees
	GLfloat wobble_period; // wobble up/down period, in minutes
	GLfloat zoom_amp; // radial zoom amplitude
	GLfloat zoom_period; // zoom in/out period, in minutes

	GLfloat rotation_offset; // so that we can rotate sphere
	GLfloat rotation_speed; // degrees per minute

	//------------ new clean members -----
	
	// view control
	void rotateSphere(const int relativeRotation, const int relativeElevation);
	void zoomSphere(const int relativeZoom);
	
	// feature control
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
	
	void setFeature(const Features features, const bool enable);
	bool isFeature(const Features features);

	// observatory movement
	// (in seconds since 1970 with usec precision)
	double m_ObservatoryDrawTimeLocal;
};

/* Constellation & star coordinates are in starlist.C */
extern float star_info[][2];
extern int Nstars;

/* Pulsar coordinates are in pulsar_list.C */
extern float pulsar_info[][2];
extern int Npulsars;

/* SNR coordinates are in snr_list.C */
extern float SNR_info[][2];
extern int NSNRs;

#endif /*STARSPHERE_H_*/
