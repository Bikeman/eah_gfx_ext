 /***************************************************************************
 * Star Sphere graphics for Einstein@Home Screen Saver  
 *
 * This file contains graphics routines to draw the stars, constellations,
 * pulstars and SNRs, and the GW Observatories.
 * 
 * Also contains the BOINC graphics API callbacks.
 *
 *****************************************************************************/
static volatile const char *CVSfileVersion= "$Id$";
/***********************************************************************
 * Copyright (C) 2004 David Hammer, Eric Myers, Bruce Allen
 * Copyright (C) 2008 Bernd Machenschalk
 * Copyright (C) 2008 Oliver Bock
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 ************************************************************************/

#ifndef _MSC_VER
using namespace std;
#include <iostream>
#endif

// needed to find OpenGL 1.4 prototypes in glext.h (alternatives?)
#define GL_GLEXT_PROTOTYPES

#include "SDL.h"

#include "starsphere.h"


/* Constellation & star coordinates are in starlist.C */
extern float star_info[][2];
extern int Nstars;

/* Pulsar coordinates are in pulsar_list.C */
extern float pulsar_info[][2];
extern int Npulsars;

/* SNR coordinates are in snr_list.C */
extern float SNR_info[][2];
extern int NSNRs;


extern "C" {
  void set_search_pos(float, float);			// in search_info.C
  void set_fraction_done(float);			// in search_info.C
}

void annotate_screen();                                 // in user_info.C
void display_search_info(double);                       // in search_info.C
void make_search_marker(GLfloat, GLfloat, GLfloat);     // in search_info.C


/**
 * Object ID's  and on/off switches.
 * (someday the keyboard can be used to turn display of items on/off)
 */
GLuint Axes=0, Stars=0, Constellations=0, Pulsars=0;
GLuint LLOmarker=0, LHOmarker=0, GEOmarker=0;
GLuint sphGrid=0, SNRs=0;

// feature bitmap
int featureFlags = 0;

/* Search position and time */ 
extern float  search_RAdeg;     // search_info.C
extern float  search_DEdeg;     // search_info.C
extern GLuint SearchMarker;     // search_info.C

// 0: dont' display, 1: display, 2: recompute position
int show_markers=MARKER_NONE;     // switch back to bool soon -EAM
extern int marker_status;    // switching to this...

/**
 * Parameters and State info:
 */
GLfloat sphRadius = 5.5;

/**
 * Viewpoint (can be changed with mouse)
 */
GLfloat viewpt_azimuth = 30.0;   // azimuth, in degrees
GLfloat viewpt_elev = 23.6;      // elevation angle, in degrees
GLfloat viewpt_radius = 7.6;     // distance out

GLfloat wobble_amp = 37.0;       // wobble amplitude, in degrees
GLfloat wobble_period = 17.0;    // wobble up/down period, in minutes
GLfloat zoom_amp = 0.00;         // radial zoom amplitude
GLfloat zoom_period = 29.0;      // zoom in/out period, in minutes

GLfloat rotation_offset = 0.0;   // so that we can rotate sphere
GLfloat rotation_speed = 180.0;  // degrees per minute

/**
 * display window size for dynamic text alignment
 * set in app+graphics_resize(), 
 * used in user_text.C - annotate_screen()
 * and search_info.C - display_search_info()
 */
float gfx_width = 0;
float gfx_height = 0;

/**
 * Timezone correction for BOINCs dtime(), only needed for Win32 
 */
#define gmt_dtime dtime


/**
 * sphVertex3D() creates a GL vertex in 3D sky sphere coordinates
 * sphVertex() creates a GL vertex on the surface of the sky sphere.
 * Use either like glVertex().
 */
void sphVertex3D(GLfloat RAdeg, GLfloat DEdeg,GLfloat radius) {
  GLfloat x, y, z;

  x =  radius * COS(DEdeg) * COS(RAdeg);
  z = -radius * COS(DEdeg) * SIN(RAdeg);
  y =  radius * SIN(DEdeg);
  glVertex3f(x, y, z);
  return;
}


void sphVertex(GLfloat RAdeg, GLfloat DEdeg) {
  sphVertex3D(RAdeg, DEdeg, sphRadius);
}


/**
 * Star Marker:
 * Makes a marker for one star at a given position and angular size.  
 */
void star_marker(float RAdeg, float DEdeg, float size) {
  glPointSize((GLfloat) size);
  glBegin(GL_POINTS);    
    sphVertex((GLfloat) RAdeg, (GLfloat) DEdeg);
  glEnd();
  return;
}


/**
 *  Create Stars: markers for each star
 */
void make_stars(){
  GLfloat mag_size;
  int i, j;
  bool is_dupe;
  int Ndupes=0;

  if (!Stars) Stars = glGenLists(1);
  glNewList(Stars, GL_COMPILE);
      glColor3f(1.0, 1.0, 1.0); 

     /**
      * At some point in the future star_info[][] will also contain
      * star magnitude and the marker size will vary with this.
      */
     for(i=0; i < Nstars; i++) {
       // same stars appear more than once in constallations so ignore dupes
       is_dupe=false;
       for(j=0; j< i; j++) {
         if ( star_info[j][0] == star_info[i][0]
           && star_info[j][0] == star_info[i][0] ) {
           is_dupe=true;
           Ndupes++;
           break;
         }
       }
       if(!is_dupe) {
         // mag_size = 0.05 + 0.50*rand()/RAND_MAX;
         mag_size = 4.0;
         star_marker(star_info[i][0],star_info[i][1], mag_size);
       }
     }
  glEndList();
  // fprintf(stderr,"make_stars: there were %d duplicate stars.\n", Ndupes);
}


/**
 *  Pulsar Markers:
 */
void make_pulsars(){
  GLfloat mag_size=3.0;
  int i;
  
  if (!Pulsars) Pulsars = glGenLists(1);
  glNewList(Pulsars, GL_COMPILE);
  glColor3f(0.80, 0.0, 0.85);        // _P_ulsars are _P_urple
  
  for(i=0; i < Npulsars; i++) {
    star_marker(pulsar_info[i][0], pulsar_info[i][1], mag_size);
  }
  glEndList();
}


/**
 * Super Novae Remenants (SNRs):
 */
void make_snrs(){
  GLfloat mag_size=3.0;
  int i;

  if (!SNRs) SNRs = glGenLists(1);
  glNewList(SNRs, GL_COMPILE);
  glColor3f(0.7, 0.176, 0.0);	  // _S_NRs are _S_ienna

  for(i=0; i < NSNRs; i++) {
    star_marker(SNR_info[i][0], SNR_info[i][1], mag_size);
  }
  glEndList();
}


/**
 * Create Constellations:
 * draws line links between pairs of stars in the list.
 */
void make_constellations(){
  GLint star_num=0;

  if (!Constellations) Constellations = glGenLists(1);
  glNewList(Constellations, GL_COMPILE);
  glLineWidth(1.0);
  glColor3f(0.7, 0.7, 0.0);	// light yellow

  glBegin(GL_LINES);            // draws lines between *pairs* of vertices
  for(star_num=0; star_num < Nstars; ++star_num) { 
    sphVertex(star_info[star_num][0],star_info[star_num][1]);
    star_num++;         
    sphVertex(star_info[star_num][0],star_info[star_num][1]);
  }
  glEnd();
  glEndList();  
}

/**
 * Create markers on sky sphere for LLO, LHO, and GEO
 * IFO corner positions are from Myers' personal GPS and are +/- 100m
 */


/**
 * RAofZenith(time, longitude)
 *
 * Computes the Right Ascention of the zenith at a given time (from
 * the Unix epoch, in seconds) at a given Longitude (in degrees). From
 * 'The Cambridge Handbook of Physics Formulas', Graham Woan, 2003
 * edition, CUP.  (NOT the first edition), p177.
 */
GLfloat RAofZenith(double T, GLfloat LONdeg) {

  // unix epoch at 12h  1/1/2000
  const double T_0 = 946728000.0;

  //  UT seconds of the day
  double T_s = fmod(T, 24.0*3600.0);

  // Julian centuries since 12h 1/1/2000 and 0h today
  double T_c = (T - T_s - T_0)/3155760000.0;

  // GMST at 0h today in seconds
  double GMST0 = 6.0*3600.0 + 41.0*60.0 + 50.54841 + (8640184.812866 + 0.093104*T_c)*T_c;

  // GMST now in seconds
  double GMST = GMST0 + 1.002738*T_s; 
  
  // longitude defined as west positive
  GLfloat alpha = (GMST/(24.0*3600.0))*360.0 - LONdeg;

  return alpha;
}


/* Time info */
double obs_dtime_drawn;         // dtime() when obs were drawn
double gmt_offset=0.0;          // dtime() correction to get GMT
bool show_gmt=true;             // show clock or not?


/**
 * Draw the observatories at their zenith positions
 */
void make_obs() {
  GLfloat Lat, Lon;             // Latitute/Longitude of IFO is
  GLfloat RAdeg, DEdeg;         // converted to RA/DEC of sky sphere position
  GLfloat radius;               // radius of sphere for obs

  GLfloat arm_len_deg=3.000;    // lenght of arms, in degrees (not to scale)
  GLfloat h2=0.400;             // slight offset for H2 arms

  double obs_gmt_dtime;         // current time in GMT to get zenith position

  obs_gmt_dtime = 0.0; //FIXME: gmt_dtime();  // Current GMT time
  obs_dtime_drawn = 0.0; //FIXME:dtime();    // dtime() not gmt_dtime(), for difference
  gmt_offset = obs_gmt_dtime - obs_dtime_drawn;   // save for GMT display

  radius = 1.0*sphRadius;       // radius of sphere on which they are drawn
  
  float lineSize = 4.0;

  /**
   * LIGO Livingston Observatory:
   */

  Lat= 30.56377;
  Lon= 90.77408;

  RAdeg= RAofZenith(obs_gmt_dtime, Lon);
  DEdeg=  Lat;

  if (!LLOmarker) LLOmarker = glGenLists(1);
  glNewList(LLOmarker, GL_COMPILE);
	  glColor3f(0.0, 1.0, 0.0);
	  glLineWidth(lineSize);
	  glBegin(GL_LINE_STRIP);
	    //  North/South arm:
	    sphVertex3D(RAdeg, DEdeg-arm_len_deg, radius);
	    sphVertex3D(RAdeg, DEdeg,radius);
	    // East/West arm:
	    sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
	  glEnd();
	  // arm joint H2
	  glPointSize((GLfloat) lineSize);
	  glBegin(GL_POINTS);    
	  	sphVertex3D(RAdeg, DEdeg, radius);
	  glEnd();
  glEndList();

  /**
   * LIGO Hanford Observatory: H1 and H2
   */

  Lat=  46.45510;
  Lon= 119.40627;

  RAdeg= RAofZenith(obs_gmt_dtime, Lon);
  DEdeg=  Lat;
  
  if (!LHOmarker) LHOmarker = glGenLists(1);
  glNewList(LHOmarker, GL_COMPILE);
	  glColor3f(0.0, 0.0, 1.0);
	  glLineWidth(lineSize);
	  glBegin(GL_LINE_STRIP);
	    // North/South arm:
	    sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
	    sphVertex3D(RAdeg, DEdeg, radius);
	    // East/West arm:
	    sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
	  glEnd();
	  glBegin(GL_LINE_STRIP);
	    // North/South arm, H2:
	    sphVertex3D(RAdeg-h2, DEdeg+arm_len_deg/2.0+h2/2.0, radius);
	    sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
	    // East/West arm, H2;
	    sphVertex3D(RAdeg-arm_len_deg/2.0-h2, DEdeg+h2/2.0, radius);
	  glEnd();
	  // arm joint H1
	  glPointSize((GLfloat) lineSize);
	  glBegin(GL_POINTS);    
	  	sphVertex3D(RAdeg, DEdeg, radius);
	  glEnd();
	  // arm joint H2
	  glPointSize((GLfloat) lineSize);
	  glBegin(GL_POINTS);    
	  	sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
	  glEnd();
  glEndList();
  
  /**
   *  GEO600 Interferometer:
   */

  Lat= 52.24452;
  Lon= -9.80683;
  arm_len_deg=1.50;   // not to scale

  RAdeg= RAofZenith(obs_gmt_dtime, Lon);
  DEdeg=  Lat;
  
  if (!GEOmarker) GEOmarker = glGenLists(1);
  glNewList(GEOmarker, GL_COMPILE);
  glColor3f(1.0, 0.0, 0.0);
	  glLineWidth(lineSize);
	  glBegin(GL_LINE_STRIP);
	    // North/South arm:
	    sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
	    sphVertex3D(RAdeg, DEdeg, radius);
	    // West/East arm:
	    sphVertex3D(RAdeg+arm_len_deg, DEdeg, radius);
	  glEnd();
	  // arm joint
	  glPointSize((GLfloat) lineSize);
	  glBegin(GL_POINTS);    
	  	sphVertex3D(RAdeg, DEdeg, radius);
	  glEnd();
  glEndList();

  return;
}


/**
 * XYZ coordinate axes: (if we want them - most useful for testing)
 */
void make_axes(){
    GLfloat axl=10.0;

    if (!Axes) Axes = glGenLists(1);
    glNewList(Axes, GL_COMPILE);
    glLineWidth(2.0);

    glBegin(GL_LINES);
      glColor3f(1.0, 0.0, 0.0);
      glVertex3f(-axl, 0.0, 0.0);
      glVertex3f( axl, 0.0, 0.0);

      glColor3f(0.0, 1.0, 0.0);
      glVertex3f(0.0, -axl, 0.0);
      glVertex3f(0.0,  axl, 0.0);

      glColor3f(0.0, 0.0, 1.0);
      glVertex3f(0.0, 0.0, -axl);
      glVertex3f(0.0, 0.0,  axl);
  glEnd();
  glEndList();
}


/**
 * RA/DEC coordinate grid on the sphere
 */
void make_globe(){
  int hr, j, i, iMax=100;
  GLfloat RAdeg, DEdeg;

  if (!sphGrid) sphGrid = glGenLists(1);
  glNewList(sphGrid, GL_COMPILE);
  glLineWidth(1.0);

  // Lines of constant Right Ascencion (East Longitude)

  for(hr=0; hr<24; hr++) {
    RAdeg=hr*15.0;
	glColor3f(0.25, 0.25, 0.25);
	if(hr==0)   glColor3f(0.55, 0.55, 0.55);
    glBegin(GL_LINE_STRIP);  
      for(i=0; i<=iMax; i++) {
        DEdeg = i*180.0/iMax - 90.0;
        sphVertex(RAdeg, DEdeg);
      }
    glEnd();
  }


  // Lines of constant Declination (Lattitude)

  for(j=1; j<=12; j++){
    DEdeg = 90.0 - j*15.0;
    glBegin(GL_LINE_STRIP);  
    for(i=0; i<=iMax; i++) {
      RAdeg = i*360.0/iMax;
      sphVertex(RAdeg, DEdeg);
    }
    glEnd();
  }
  glEndList();
}


/**
 * BOINC Graphics callbacks: 
 */

// Graphics state info:
float aspect = 4 / 3;

/**
 * Window resize/remap
 * (FWIW, this is called *before* app_graphics_init(), 
 * when window is first mapped)
 */
void app_graphics_resize(int width, int height) {

	/* Adjust aspect ratio and projection */
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);

	gfx_width = (float)width;
	gfx_height = (float)height;  
	aspect = gfx_width / gfx_height;
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(95.0, aspect, 0.50, 25.0);
	glMatrixMode(GL_MODELVIEW);
	
	// Update in case time (zone?) change
	gmt_offset = 2; //FIXME: gmt_dtime() - dtime(); 
}


/**
 *  What to do when graphics are "initialized".
 */
void app_graphics_init() {

  // Drawing setup:
  glClearColor(0.0, 0.0, 0.0, 0.0);	// background is black

  // some polishing
  glShadeModel(GL_SMOOTH);
  glEnable(GL_POINT_SMOOTH);
  glEnable(GL_LINE_SMOOTH);
  
  glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_FOG_HINT, GL_DONT_CARE);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Enable depth buffering for 3D graphics */
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);      
  glDepthFunc(GL_LEQUAL);               
 
  /* Fog aids depth perception */
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_EXP2);
  glFogf(GL_FOG_DENSITY, 0.085);

  /* Set initial perspective projection */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(95.0, aspect, 0.50, 25.0);
  glMatrixMode(GL_MODELVIEW);

   /* Create pre-drawn display lists */
  make_stars();    
  make_constellations(); 
  make_pulsars();
  make_snrs();
  make_axes();
  make_globe();
  make_obs();
  
  setFeature(STARS, true);
  setFeature(CONSTELLATIONS, true);
  setFeature(PULSARS, true);
  setFeature(OBSERVATORIES, true);
  setFeature(SNRS, true);
  setFeature(GLOBE, true);
  setFeature(SEARCHINFO, true);
  setFeature(LOGO, true);

  glDisable(GL_CLIP_PLANE0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glFlush();
}


/**
 * Rendering routine:  this is what does the drawing:   
 */
void app_graphics_render(int xs, int ys, double time_of_day){
  GLfloat xvp, yvp, zvp, vp_theta, vp_phi, vp_rad; 
  GLfloat Zrot = 0.0, Zobs=0.0; 
  double revs,  t, dt = 0;
  static double start_time=-1.0, last_time=-1.0;
  static const double rad2deg = 180.0/PI;

  // Calculate the real time t since we started (or reset) and the
  // time dt since the last render() call.    Both may be useful
  // for timing animations.  Note that time_of_day is dtime().

  if ( start_time < 0.0 )  start_time = time_of_day;
  t = time_of_day - start_time;

  if (last_time < 0.0)  last_time = time_of_day - 0.01; 
  dt = time_of_day - last_time; 

  last_time = time_of_day;      // remember for next time

  // Now determine the rotation angle based on the time since start
  // It is negative to get the rotation direction correct (the sun
  // rises in the East, so the sky sphere rotates E to W).

  Zrot = t*rotation_speed/60.0;   
  revs = Zrot/360.0;
  Zrot = -360.0 * ( revs - (int)revs);

  // And start drawing...
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // First, write user text to screen,  from user_text.C

  annotate_screen();
  if(isFeature(SEARCHINFO)) display_search_info(time_of_day);


  // Now draw the scene...

  glLoadIdentity();
 
  // Vary the viewpoint with both a long period wobble of the elevation 
  // of the view and a longer period zoom in/out that might even penetrate
  // The starsphere for a brief time.   Increase the power in pow(,) to 
  // make the visit inside briefer.

  vp_theta = 90.0 - viewpt_elev + wobble_amp*sin(PI2*t/(wobble_period*60.0));
  vp_phi = viewpt_azimuth;
  vp_rad = viewpt_radius - zoom_amp*sin(PI2*t/(zoom_period*60.0));
  if(vp_rad <0.0) vp_rad = 0.0;         // cannot pass origin (confusing)
       // TRIED THIS TOO: -zoom_amp*pow(fabs(sin(PI2*t/(zoom_period*60.0))),3);
  xvp = vp_rad * SIN(vp_theta) * SIN(vp_phi);
  zvp = vp_rad * SIN(vp_theta) * COS(vp_phi);
  yvp = vp_rad * COS(vp_theta);

  gluLookAt ( xvp,  yvp, zvp,      // eyes position
               0.0, 0.0, 0.0,                   // looking toward here
               0.0, 1.0, 0.0);                  // which way is up?  y axis!

  // Draw axes before any rotation so they stay put
  if(isFeature(AXES)) glCallList(Axes);

  
  // draw 3D vectorized text
//  char text[] = "Einstein@Home";
//  glColor3f(1.0, 0.0, 0.0);
  
//  glPushMatrix();  		
//  		
//  		// third: relocate string to upper left corner
//  		//glTranslatef(-5.0, 0.0, 0.0);
//  		
//  		
//  	  	// second: reverse viewpoint rotation for text display (z not needed as long as y is up-vector!)
//  		GLfloat xRotate = 0;
//  		GLfloat yRotate = 0;
//  		
//
// 		// reverse x-rotation
//  		if( yvp == 0 ) {
//  			xRotate = 0.0;
//  		}
//  		else if( zvp == 0 ) {
//  			xRotate = 90.0 * abs(yvp)/yvp;
//  		}
//  		else {
//  			xRotate = -atan(yvp/zvp) * rad2deg;
//  			yRotate = zvp<0 ? 180 : 0;
//  		}
//  		glRotatef(xRotate, 1.0, 0.0, 0.0);
//  		glRotatef(yRotate, 0.0, 1.0, 0.0);
//
//  		// reverse y-rotation
//  		if( xvp == 0 ) {
//  			yRotate = 0.0;
//  		}
//  		else if( zvp == 0 ) {
//  			yRotate = 90.0 * abs(xvp)/xvp;
//  		}
//  		else {
//  			yRotate = atan(xvp/zvp) * rad2deg;
//  			yRotate -= zvp<0 ? 180 : 0;
//  		}
//  		glRotatef(yRotate, 0.0, 1.0, 0.0);  		
//
//  		
//  		// first: center string around origin (for following rotations, see above!)
//  		float lowerLeftX = 0;
//  	  	float lowerLeftY = 0;
//  	  	float lowerLeftZ = 0;
//  	  	float upperRightX = 0;
//  	  	float upperRightY = 0;
//  	  	float upperRightZ = 0;
//  	  	font->BBox(text, lowerLeftX, lowerLeftY, lowerLeftZ, upperRightX, upperRightY, upperRightZ);
//  		glTranslatef(-(upperRightX-lowerLeftX) / 2, -(upperRightY-lowerLeftY) / 2, -(upperRightZ-lowerLeftZ) / 2);  		
//
//  		
//  		// render text
//		font->FaceSize(1);
//		font->Depth(0.05);
//		font->Render(text);
//  glPopMatrix();
  
  // Draw the sky sphere, with rotation:

  glPushMatrix(); 
    glRotatef(Zrot - rotation_offset ,  0.0, 1.0, 0.0);

    /* stars, pulsars, supernovae */

    if(isFeature(STARS))          glCallList(Stars);
    if(isFeature(PULSARS))        glCallList(Pulsars);
    if(isFeature(SNRS))           glCallList(SNRs);
    if(isFeature(CONSTELLATIONS)) glCallList(Constellations);
    if(isFeature(GLOBE))          glCallList(sphGrid);

    /* search position marker */

    if (show_markers){
      if (show_markers==MARKER_NEW) {   // need to reposition the marker?
        make_search_marker(search_RAdeg, search_DEdeg, 0.50);
        show_markers=MARKER_SHOW;
      }
      glCallList(SearchMarker);
    }


    /* Observatories move an extra 15 degrees/hr since they were drawn */

    if(isFeature(OBSERVATORIES)) {
      glPushMatrix();
        Zobs = (time_of_day-obs_dtime_drawn) * 15.0/3600.0;
        glRotatef(Zobs, 0.0, 1.0, 0.0);            
        glCallList(LLOmarker);
        glCallList(LHOmarker);
        glCallList(GEOmarker);
      glPopMatrix();
    }

  glPopMatrix();

  
  // draw 2D rasterized text

  font->FaceSize(24);
  static int yOffset = font->Ascender() + font->Descender();
  
  if(isFeature(LOGO))
  {  
	  GLfloat xStartPosLeft = 10.0;
	  GLfloat yStartPos = ys - 30.0;
	  
	  glColor4f(1.0, 1.0, 0.0, 1.0);
	  glWindowPos2f(xStartPosLeft, yStartPos);
	  font->FaceSize(24);	  
	  font->Render("Einstein@Home");
	  glColor4f(1.0, 1.0, 1.0, 0.5);
	  glWindowPos2f(xStartPosLeft, yStartPos - yOffset - 5);
	  font->FaceSize(14);	  
	  font->Render("World Year of Physics 2005");	  
  }
  
  if(isFeature(SEARCHINFO))
  {
	  GLfloat xStartPosLeft = 10.0;
	  GLfloat xStartPosRight = xs - 200.0;
	  GLfloat yStartPos = 80.0;
	  	  
	  // left info block	  
	  glColor4f(1.0, 1.0, 0.0, 0.5);
	  glWindowPos2f(xStartPosLeft, yStartPos);
	  font->FaceSize(16);
	  font->Render("BOINC Statistics");
	  
	  // TODO: how to write multiline?
	  glColor4f(1.0, 1.0, 1.0, 0.5);
	  font->FaceSize(12);
	  glWindowPos2f(xStartPosLeft, yStartPos - yOffset);
	  font->Render("User: Oliver");
	  glWindowPos2f(xStartPosLeft, yStartPos - 2*yOffset);
	  font->Render("Team: Albert-Einstein-Institut");
	  glWindowPos2f(xStartPosLeft, yStartPos - 3*yOffset);
	  font->Render("Project credits: 12.000");
	  glWindowPos2f(xStartPosLeft, yStartPos - 4*yOffset);
	  font->Render("Project RAC: 145.00");
	  
	  // right info block	  
	  glColor4f(1.0, 1.0, 0.0, 0.5);
	  glWindowPos2f(xStartPosRight, yStartPos);
	  font->FaceSize(16);
	  font->Render("Search Parameters");
	  
	  // TODO: how to write multiline?
	  glColor4f(1.0, 1.0, 1.0, 0.5);
	  font->FaceSize(12);
	  glWindowPos2f(xStartPosRight, yStartPos - yOffset);
	  font->Render("Right ascension: 180.00 deg");
	  glWindowPos2f(xStartPosRight, yStartPos - 2*yOffset);
	  font->Render("Declination: 45.00 deg");
	  glWindowPos2f(xStartPosRight, yStartPos - 3*yOffset);
	  font->Render("Frequency: 850.05 Hz");
	  glWindowPos2f(xStartPosRight, yStartPos - 4*yOffset);
	  font->Render("Spindown rate: 3.25 * 10^-10 Hz/s");
  }


  glFlush();
  
  SDL_GL_SwapBuffers();
}

/**
 * View control
 */
void rotateSphere(const int relativeRotation, const int relativeElevation)
{
	// elevation
    viewpt_elev += relativeElevation/10.0;
    if( viewpt_elev >  89.0 ) viewpt_elev = +89.0;
    if( viewpt_elev < -89.0 ) viewpt_elev = -89.0;

    // rotation
    rotation_offset -= relativeRotation/10.0;   
}

void zoomSphere(const int relativeZoom)
{
	// zoom
    viewpt_radius -= relativeZoom/10.0;
    if (viewpt_radius > 15.0) viewpt_radius = 15.0;
    if (viewpt_radius < 0.5)  viewpt_radius = 0.5;
}


/**
 * Feature control
 */
void setFeature(const FEATURES feature, const bool enable)
{
	featureFlags = enable ? (featureFlags | feature) : (featureFlags & ~feature);
}

bool isFeature(const FEATURES feature)
{
	return ((featureFlags & feature) == feature ? true : false);
}
