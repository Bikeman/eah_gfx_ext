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
#include <FTGLPolygonFont.h>

#include <util.h>

#include "AbstractGraphicsEngine.h"

#include "oglft/include/OGLFT.h"

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

class Starsphere : public AbstractGraphicsEngine
{
public:
	virtual ~Starsphere();

	// core methods
	void initialize(const int width, const int height, const Resource *font);
	void resize(const int width, const int height);
	void render(const double timeOfDay);

	// event handling
	void mouseButtonEvent(const int positionX, const int positionY, const int buttonPressed);
	void mouseMoveEvent(const int deltaX, const int deltaY, const int buttonPressed);
	void keyboardPressEvent(const int keyPressed);
	
	// update HUD content
	virtual void refreshBOINCInformation();
	
protected:
	/// Contructor (protected since this an abstract class)	
	Starsphere();
	

	// resource handling
	FTFont *m_PolygonFont;
	OGLFT::TranslucentTexture* face;
	
	// Graphics state info:
	float aspect;
	
	// HUD text rendering config (maybe overridden in subclasses)
	GLfloat m_XStartPosLeft;
	GLfloat m_YStartPosTop;
	GLfloat m_FontScaleLarge;
	GLfloat m_FontScaleMedium;
	GLfloat m_FontScaleSmall;
	GLfloat m_YOffsetLarge;
	GLfloat m_YOffsetMedium;
	
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
	GLuint LLOmarker, LHOmarker, GEOmarker;
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
	
	// application specific rendering
	virtual void renderSearchInformation() = 0;
	
	// view control
	void rotateSphere(const int relativeRotation, const int relativeElevation);
	void zoomSphere(const int relativeZoom);
	
	// feature control
	void setFeature(const int features, const bool enable);
	bool isFeature(const int features);

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
