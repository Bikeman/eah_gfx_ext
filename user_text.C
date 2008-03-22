/*****************************************************************************
 * Screen Text for Einstein@Home - shows user name, credit, team, etc...
 *
 *****************************************************************************/
static volatile const char *CVSfileVersion= "$Id$";
/***********************************************************************
 * Copyright (C) 2004 David Hammer, Eric Myers, Bruce Allen
 * Copyright (C) 2008 Bernd Machenschalk
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

#include "starsphere.h"

#define USER_NAME   "Oliver"
#define USER_CREDIT 1000.0
#define TEAM_NAME   "AEI"
#define HOST_CREDIT 2000.0

/*******************************************
 *  Write text from string at current raster position using given font
 */

void write_text(char *string, void *font){
  int len, i;
  len = (int) strlen(string);       
  for (i = 0; i < len; i++) {       
    //TODO: glutBitmapCharacter(font, string[i]);
  }
}


float baselineskip;

void annotate_screen(){
  char bufr[256];
  GLfloat x, y;
  static GLfloat xloc = .72;
  static GLfloat yloc = .95;

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
      
      /* calculate based on window size */
      if (gfx_height > 0) {
	baselineskip = 14./gfx_height;
	yloc = 1.0 - (24. / gfx_height); 
      }
      
      /* Overall Title */
      
      xloc=0.02;
      glColor3f(1.0, 1.0, 1.0);                 // WHITE
      glRasterPos2f(xloc, yloc);                // position
      // FIXME: write_text("Einstein@Home",  FONT_RM24);  // text

      /* World Year of Physics 2005 */

      yloc -= baselineskip;
      glColor3f(0.5, 0.5, 1.0);                 // light blue
      glRasterPos2f(xloc, yloc);                // position
      // FIXME: write_text("World Year of Physics 2005",  FONT_SS12);  // text


#ifdef BOINC_APIV6
      if(shmem) {
#endif

      x = 0.02;
      y = 4.0*baselineskip;

      glRasterPos2f(x, y);                              // position
      // FIXME: write_text("User: ",  FONT_SS12);                 // text
      // FIXME: write_text(USER_NAME,  FONT_SS12);      // value

      y -= baselineskip;
      glRasterPos2f(x, y);                              // position
      if (USER_CREDIT<100.0)
	sprintf(bufr, "Total credit: %7.2f", USER_CREDIT);
      else if (USER_CREDIT<1000.0)
	sprintf(bufr, "Total credit: %7.1f", USER_CREDIT);
      else
	sprintf(bufr, "Total credit: %7.0f", USER_CREDIT);

      // FIXME: write_text(bufr,  FONT_SS12);                     // value

      y -= baselineskip;
      glRasterPos2f(x, y);                              // position
      if (HOST_CREDIT<100.0)
	sprintf(bufr, "Host credit:  %7.2f ", HOST_CREDIT);
      else if (HOST_CREDIT<1000.0)
	sprintf(bufr, "Host credit:  %7.1f ", HOST_CREDIT);
      else
	sprintf(bufr, "Host credit:  %7.0f ", HOST_CREDIT);

      // FIXME: write_text(bufr,  FONT_SS12);                     // value


      /* Team Info (if there is a team)*/

      if(strlen(TEAM_NAME)>0){
        y -= baselineskip;
        glRasterPos2f(x, y);                    // position
	sprintf(bufr, "Team: %s", TEAM_NAME);
	// FIXME: write_text(bufr,  FONT_SS12);            
      }

      /* restore transformations */

      glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  glPopMatrix();


}

//EOF
