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
#include "AbstractGraphicsEngine.h"
#include "GraphicsEngineFactory.h"

using namespace std;


int main(int argc, char **argv)
{
	// prepare main objects
	WindowManager window;
	ResourceFactory factory;
	AbstractGraphicsEngine *graphics = GraphicsEngineFactory::createInstance(
											GraphicsEngineFactory::Starsphere,
											GraphicsEngineFactory::EinsteinS5R3);
	
	if(!graphics) {
		cerr << "Requested graphics engine could not be found/instantiated!" << endl;
		exit(1);
	}
    
    // initialize window manager 
    if(!window.initialize()) {
    	cerr << "Window manager could not be initialized!" << endl;
    	delete graphics;
        exit(1);
    }
    
	// create font resource instance
	const Resource *fontResource = factory.createInstance("FontSansSerif");
	
	if(fontResource == NULL) {
		cerr << "Font resource could not be loaded!" << endl;
		delete graphics;
		exit(1);
	}
	
	if(fontResource->data()->size() <= 0) {
		cerr << "Font resource could not be loaded!" << endl;
		delete graphics;
		delete fontResource;
		exit(1);
	}

    window.setWindowCaption("Einstein@Home");
	
    // register starsphere as event observer
    window.registerEventObserver(graphics);

#ifdef NDEBUG
	// switch to fullscreen when in release mode
	window.toggleFullscreen();
#endif

	// pepare rendering
	graphics->initialize(window.windowWidth(), window.windowHeight(), fontResource);
	graphics->refreshBOINCInformation();

	// enter main event loop
	window.eventLoop();
	
	// clean up end exit
	window.unregisterEventObserver(graphics);
	delete graphics;
	delete fontResource;
	
	exit(0);
}
