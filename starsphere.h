/*****************************************************************************
 * Star Sphere graphics for Einstein@Home Screen Saver  
 * 
 * @(#) $Id$
 *
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

#ifndef BOINC_APP_GRAPHICS
#define BOINC_APP_GRAPHICS

// or for Unix 
#include <cmath>
#include <cstdio>
#include <cstring>

#include <GL/gl.h>
#include <GL/glu.h>

#define EAH_SHMEM_APP_NAME "Einstein@Home"

/* SIN and COS take arguments in DEGREES */
#define PI 3.14159265
#define PI2 (2*PI)
#define COS(X)   cos( (X) * PI2/360.0 )
#define SIN(X)   sin( (X) * PI2/360.0 )

/* search marker status */
#define MARKER_NONE 0
#define MARKER_SHOW 1
#define MARKER_NEW  2

// place to keep current window dimensions
extern float gfx_width, gfx_height;

// If graphics are disabled during compilation then we still need
// empty callbacks for these:
//#ifndef BOINC_APP_GRAPHICS 

void app_graphics_init();
void app_graphics_resize(int w, int h);
void app_graphics_render(int xs, int ys, double time_of_day);
void boinc_app_mouse_button(int x, int y, int which, int is_down);
void boinc_app_mouse_move(int x, int y, int left, int middle, int right);
void boinc_app_key_press(int, int);
void boinc_app_key_release(int, int);

// features
typedef enum Features {
	STARS=1,
	CONSTELLATIONS=2,
	OBSERVATORIES=4,
	XRAYS=8,
	PULSARS=16,
	SNRS=32,
	GLOBE=64,
	AXES=128,
	SEARCHINFO=256
} FEATURES;

// feature control API
void setFeature(const FEATURES features, const bool enable);
bool isFeature(const FEATURES features);


#endif
