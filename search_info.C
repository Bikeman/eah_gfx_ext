/**********************************************************************
 * Search info - set/display info about search position and time
 *               and display marker 
 *
 **********************************************************************/
static volatile const char *CVSfileVersion= "$Id$";

/***********************************************************************
 * Copyright (C) 2004 David Hammer, Eric Myers, Bruce Allen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * You should have received a copy of the GNU General Public License
 * (for example COPYING); if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 ***********************************************************************/

#include "starsphere.h"


// Functions:

void sphVertex(GLfloat RAdeg, GLfloat DEdeg);
void sphVertex3D(GLfloat, GLfloat, GLfloat);
void write_text(char *string, void *font);
extern "C" {
  void set_search_pos(float, float);
  void set_fraction_done(float);
}

#ifdef __cplusplus
extern "C" {
#endif
  double fraction_done;
#ifdef __cplusplus
}
#endif



/* Search position and time */

float  search_RAdeg=0.0;
float  search_DEdeg=100.0;   // initial value > 90 means not set yet
float  search_marker_size;   // to remember
GLuint SearchMarker=0;          

int Nsearched=0;             // Number of positions searched


/* Previously searched positions */

#define MAX_SEARCHED_LIST  100
GLuint SearchedList[MAX_SEARCHED_LIST];
bool searched_list_wrapped=false;
int i_searched_last=0;


/* Externs from other files */

extern bool show_gmt;        // starsphere.C
extern double gmt_offset;    // starsphere.C
extern GLfloat sphRadius;    // starsphere.C


/******************************
 *  draw_gunsight(RA,DE,size) draws a "gunsight"  marker of the given size
 *  at the given position.  
 */

void draw_gunsight(GLfloat RAdeg, GLfloat DEdeg, GLfloat size){
  GLfloat x, y;
  GLfloat r1, r2, r3;
  float theta;
  int i, Nstep=20;

  // r1 is inner circle, r2 is outer circle, r3 is crosshairs
  r1 = size , r2=3*size, r3=4*size;

  glPushMatrix(); 
    // First rotate east  to the RA position around y
    glRotatef(RAdeg,  0.0, 1.0, 0.0);
    // Then rotate up to DEC position around z (not x)
    glRotatef(DEdeg,  0.0, 0.0, 1.0); 

    glBegin(GL_LINE_LOOP);        // Inner circle
    for(i=0; i<Nstep ; i++){
      theta = i*360.0/Nstep;
      x = r1*COS(theta);  
      y = r1*SIN(theta);  
      sphVertex(x,y);
    }
    glEnd();

    glBegin(GL_LINE_LOOP);        // Outer circle
    for(i=0; i<Nstep ; i++){
      theta = i*360.0/Nstep;
      x = r2*COS(theta); 
      y = r2*SIN(theta); 
      sphVertex(x,y);
    }
    glEnd();

    /*  Arms that form the gunsight */
    glBegin(GL_LINES);            // draws lines between *pairs* of vertices
    //  North arm:
    sphVertex(0.0, +r1);     sphVertex(0.0, +r3);
    //  South arm:
    sphVertex(0.0, -r1);     sphVertex(0.0, -r3);
    // East arm:
    sphVertex(-r1, 0.0);     sphVertex(-r3, 0.0);
    // West arm:
    sphVertex(+r1, 0.0);     sphVertex(+r3, 0.0);

    glEnd();

    // back to previous coordinates
  glPopMatrix(); 
  return;
}



/******************************
 *  make_search_marker() creates a marker object at the search position
 */

void make_search_marker(GLfloat RAdeg, GLfloat DEdeg, GLfloat size) {

  // delete any existing marker, then create a new one
  if (SearchMarker) glDeleteLists(SearchMarker,SearchMarker);
  SearchMarker = glGenLists(1);
  glNewList(SearchMarker, GL_COMPILE);
    glLineWidth(3.0);                           
    glColor3f(1.0, 0.5, 0.0);                   // Orange
    draw_gunsight(RAdeg, DEdeg, size);
    if (0){          // searchlight line out to marker (OFF!)
      glBegin(GL_LINES);
      sphVertex3D(RAdeg, DEdeg, 0.50*sphRadius);
      sphVertex3D(RAdeg, DEdeg, 0.95*sphRadius);
      glEnd();
    }
  glEndList();
  return;
}



/*************************
 *  Set search position:
 */

extern int  show_markers;
int  marker_status=MARKER_NONE;
extern bool show_search_info;

void set_search_pos(float RAdeg, float DEdeg) {
  search_RAdeg = RAdeg;
  search_DEdeg = DEdeg;
  show_markers=MARKER_NEW; // change this back to bool on/off soon. -EAM
  marker_status=MARKER_NEW;// flag that we should make a new marker
  show_search_info=true;
  Nsearched++;
}

void set_fraction_done(float frdone) {
  fraction_done = frdone;
}

/******************************
 * Display run/search conditions
 */

// assigned a value when annotate_screen() is called
extern float baselineskip;

void display_search_info(double time_of_day){
  char bufr[255];
  GLfloat x, y=0.95;
  static int search_text_width = 0;


  // First we have to save both MODELVIEW and PROJECTION transformations:

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();       
    glLoadIdentity();     
    glMatrixMode(GL_PROJECTION);        // save PROJECTION
    glPushMatrix(); 
      glLoadIdentity();

      /* Orthographics projection to make screen coordinates run from
       * 0.0 to 1.0 in both x and y, with z=0.0 
       */

      glOrtho(0.0, 1.0,   0.0, 1.0,  -1.0, 1.0);
	  /* first time, get length of title string in pixels */
	  if (search_text_width == 0) {
		  // FIXME: search_text_width      = glutBitmapLength( FONT_SS12, (const unsigned char*)"Search information:");
		  // FIXME: int search_text_width2 = glutBitmapLength( FONT_RM24, (const unsigned char*)"88:88:88");
		  // FIXME: if (search_text_width2>search_text_width) search_text_width=search_text_width2;
		  // FIXME: int search_text_width3 = glutBitmapLength( FONT_SS12, (const unsigned char*)"Percent done: 99.999%");
		  // FIXME: if (search_text_width3>search_text_width) search_text_width=search_text_width3;
	    
	  }
  
	  if (gfx_width > 0) {
	    x = 1.0 - ( (5+(float)search_text_width) / gfx_width );
	  } else {
		  x = .8;
	  }
	  if (x < .5) {
		  x = .5;
	  }

      if (gfx_height > 0) {
	y = 1.0 - (24. / gfx_height); 
      }

      
      glRasterPos2f(x, y);                           // position
      // FIXME: time_t timenow=time(0);
      // FIXME: struct tm* tm=localtime(&timenow);
      // FIXME: strftime(bufr, sizeof(bufr)-1, "%H:%M:%S", tm);
      // FIXME: write_text(bufr,  FONT_RM24);


      y = 4.0*baselineskip;

      /* Search Information */	

      glColor3f(0.5, 0.5, 1.0);                 // WHITE
      glRasterPos2f(x, y);                              // position
      // FIXME: write_text("Search information:",  FONT_SS12);    // text

      y -= baselineskip;
      glRasterPos2f(x, y);                              // position
      sprintf(bufr, "  RA: % 7.2f", search_RAdeg);
      // FIXME: write_text(bufr,  FONT_SS12);                     // value

      y -= baselineskip;
      glRasterPos2f(x, y);                              // position
      sprintf(bufr, "  DE: % 7.2f", search_DEdeg);
      // FIXME: write_text(bufr,  FONT_SS12);                     // value

      if(fraction_done > 0.0001){
        y -= baselineskip;
        glRasterPos2f(x, y);                            // position
        sprintf(bufr, "Percent done: %05.3f%%", 100.0*fraction_done);
        // FIXME: write_text(bufr,  FONT_SS12);                   // text
      }



      #ifdef PIRATES

      // Currently only shows number of positions searched since
      // the start or last restart, not total for WU.  
      y -= baselineskip;
      glRasterPos2f(x, y);                              // position
      sprintf(bufr, " Searched: %9d", Nsearched);
      write_text(bufr,  FONT_SS12);                     // value

      // Current GMT time hh:mm:ss
      if(show_gmt) {
        int today, s, h, m;
        today = (int) (time_of_day + gmt_offset) % 86400;   // seconds into today
        s = today % 60;
        m = (today / 60) % 60;
        h = (today / 3600);
        y -= baselineskip;
        glRasterPos2f(x, y);                
        sprintf(bufr, " %2.2d:%2.2d:%2.2d GMT", h, m, s);
        write_text(bufr,  FONT_SS12);                     // value
      }

      #endif

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}


//EOF
