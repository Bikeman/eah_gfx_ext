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


#include "WindowManager.h"
#include "ResourceFactory.h"
#include "Starsphere.h"

using namespace std;


int main(int argc, char **argv)
{
    Starsphere graphics;
    WindowManager window;
    
    if(!window.initialize()) {
        exit(1);
    }
    
    window.setWindowCaption("Einstein@Home");
    window.registerEventObserver(&graphics);
	
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
	
	graphics.initialize(window.windowWidth(), window.windowHeight(), fontResource);
	graphics.render(0);
	
#ifdef NDEBUG
	window.toggleFullscreen();
#endif
	
	window.eventLoop();
	
	window.unregisterEventObserver(&graphics);
	delete fontResource;
	
	exit(0);
}
