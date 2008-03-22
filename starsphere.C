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

#include "SDL/SDL.h"

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

bool show_stars=true, show_constellations=true, show_obs=true;
bool show_xrays=true, show_pulsars=true, show_snrs=true;
bool show_globe=false, show_axes=false;

bool show_search_info=false;    // not unless search_pos is set

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
  glLineWidth(4.0);
  glBegin(GL_LINES);            // draws lines between *pairs* of vertices
     //  North/South arm:
    sphVertex3D(RAdeg, DEdeg,radius);
    sphVertex3D(RAdeg, DEdeg-arm_len_deg, radius);
    // East/West arm:
    sphVertex3D(RAdeg, DEdeg,radius);
    sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);
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
  glLineWidth(4.0);
  glBegin(GL_LINES);            // draws lines between *pairs* of vertices
     //  North/South arm:
    sphVertex3D(RAdeg, DEdeg, radius);
    sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
    // East/West arm:
    sphVertex3D(RAdeg, DEdeg, radius);
    sphVertex3D(RAdeg-arm_len_deg, DEdeg, radius);

     //  North/South arm, H2:
    sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
    sphVertex3D(RAdeg-h2, DEdeg+arm_len_deg/2.0+h2/2.0, radius);
    // East/West arm, H2;
    sphVertex3D(RAdeg-h2, DEdeg+h2/2.0, radius);
    sphVertex3D(RAdeg-arm_len_deg/2.0-h2, DEdeg+h2/2.0, radius);
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
  glLineWidth(4.0);
  glBegin(GL_LINES);            // draws lines between *pairs* of vertices
     //  North/South arm:
    sphVertex3D(RAdeg, DEdeg, radius);
    sphVertex3D(RAdeg, DEdeg+arm_len_deg, radius);
    // West/East arm:
    sphVertex3D(RAdeg, DEdeg, radius);
    sphVertex3D(RAdeg+arm_len_deg, DEdeg, radius);
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
	glColor3f(0.6, 0.6, 0.0);
	if(hr==0)   glColor3f(0.9, 0.9, 0.0);
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
  glEnable(GL_POINT_SMOOTH);		// Anti-aliasing points (rounded)

  // First, write user text to screen,  from user_text.C

  annotate_screen();
  if(show_search_info) display_search_info(time_of_day);


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
  if(show_axes) glCallList(Axes);


  // Draw the sky sphere, with rotation:

  glPushMatrix(); 
    glRotatef(Zrot - rotation_offset ,  0.0, 1.0, 0.0);

    /* stars, pulsars, supernovae */

    if(show_stars)          glCallList(Stars);
    if(show_pulsars)        glCallList(Pulsars);
    if(show_snrs)           glCallList(SNRs);
    if(show_constellations) glCallList(Constellations);
    if(show_globe)          glCallList(sphGrid);

    /* search position marker */

    if (show_markers){
      if (show_markers==MARKER_NEW) {   // need to reposition the marker?
        make_search_marker(search_RAdeg, search_DEdeg, 0.50);
        show_markers=MARKER_SHOW;
      }
      glCallList(SearchMarker);
    }


    /* Observatories move an extra 15 degrees/hr since they were drawn */

    if(show_obs) {
      glPushMatrix();
        Zobs = (time_of_day-obs_dtime_drawn) * 15.0/3600.0;
        glRotatef(Zobs, 0.0, 1.0, 0.0);            
        glCallList(LLOmarker);
        glCallList(LHOmarker);
        glCallList(GEOmarker);
      glPopMatrix();
    }

  glPopMatrix();
  glFlush();
  
  SDL_GL_SwapBuffers();
}


/**
 * Preferences: (will go to preferences.C someday...)
 */ 
void app_graphics_reread_prefs(){}


/**
 * User input via mouse and keyboard
 * (to be moved to a separate file some day)
 */
// mouse&keyboard state:
int mouse_x=0, mouse_y=0;               
bool mouse_down = false;


/**
 * Controling the view with the mouse:
 *   Left button zooms in/out.
 *   Right button rotates viewpoint.
 */
void boinc_app_mouse_move(int x, int y, int left, int middle, int right) {
  double dx, dy;

  //  fprintf(stderr,"APP: boinc_app_mouse_move(): state= %d, %d, %d\n",
  //      left, middle, right);

  if (left) {
    // Vertical mouse motion changes elevation angle
    dy = (y - mouse_y);
    viewpt_elev += dy/10.0;
    if( viewpt_elev >  89.0 ) viewpt_elev = +89.0;
    if( viewpt_elev < -89.0 ) viewpt_elev = -89.0;

    // Horizontal motion changes rotation angle
    dx = (x - mouse_x);
    rotation_offset -= dx/10.0;   
    mouse_down=true;
  }
  else if (right) {
    dy = (y - mouse_y);
    viewpt_radius -= dy/10.0;
    if (viewpt_radius > 15.0) viewpt_radius = 15.0;
    if (viewpt_radius < 0.5)  viewpt_radius = 0.5;
    mouse_down=true;
  }

  else if (middle){
    mouse_down=true;
  }
  else {
    mouse_down = false;
  }

  // Then remember mouse position for next time
  mouse_y = y;
  mouse_x = x;
}


/**
 * Pressing a mouse button: 
 */
void boinc_app_mouse_button(int x, int y, int button, int state) {

  mouse_x = x;
  mouse_y = y;

  if(button==0 && state) { // LEFT button
    //fprintf(stderr,"mouse_click: Left button would display help menu.\n");
  }

  if(button==2 && state) { // RIGHT button
    // fprintf(stderr,"mouse_click: Right button just remembers position.\n");
  }
}


/**
 * Pressing a key on the keyboard:
 */
bool shift_is_down=false;
bool ctrl_is_down=false;


void boinc_app_key_press(int key, int state ) {}

void boinc_app_key_release(int, int) {}
