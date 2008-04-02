/****************************************************************************
 *  Main program for a BOINC graphics application
 *  $Id$
 *
 *  Graphics routines are in the separate file starsphere.C and starlist.C
 *  Core of this program comes from lalane.C
 *  
 *  Eric Myers <myers@vassar.edu>  -  3 August 2004
 *  Department of Physics and Astronomy, Vassar College, Poughkeepsie, New York
 *  @(#) $Version: 3.13$
 *****************************************************************************/

/*
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
 *
 */

#include <iostream>
#include <stdexcept>


#include "starsphere.h"
#include "WindowManager.h"
#include "ResourceFactory.h"

using namespace std;

// ugly globals, will eventually factored out into private members
FTFont *font = NULL;

int main(int argc, char **argv) {
	
    WindowManager window;
    if(!window.initialize()) {
        exit(1);
    }
    
    window.setWindowCaption("Einstein@Home");
	
	// prepare resource factory
	ResourceFactory factory;
	
	// create font resource instance
	const Resource *fontResource = factory.createInstance("FontSansSerif");
	
	if(fontResource == NULL) {
		cerr << "Font resource could not be loaded!" << endl;
		exit(1);
	}
	
	if(fontResource->data()->size() == 0) {
		cerr << "Font resource could not be loaded!" << endl;
		exit(1);
	}
	
	// create font instance using font resource (base address + size)
//	font = new FTGLBitmapFont((&fontResource->data()->at(0)), fontResource->data()->size());
//	font = new FTGLPixmapFont((&fontResource->data()->at(0)), fontResource->data()->size());
//	font = new FTGLOutlineFont((&fontResource->data()->at(0)), fontResource->data()->size());
	font = new FTGLPolygonFont((&fontResource->data()->at(0)), fontResource->data()->size());
//	font = new FTGLExtrdFont((&fontResource->data()->at(0)), fontResource->data()->size());
//	font = new FTGLTextureFont((&fontResource->data()->at(0)), fontResource->data()->size());
	
	font->CharMap(ft_encoding_unicode);
//	font->Depth(0.05);
	font->FaceSize(1);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);	

#ifndef DEBUG
	window.toggleFullscreen();
#endif

	app_graphics_resize(window.windowWidth(), window.windowHeight());
	app_graphics_init();
	app_graphics_render(window.windowWidth(), window.windowHeight(), 0);
	
	window.eventLoop();
	
	if (font) delete font;
	delete fontResource;
	
	exit(0);
}
