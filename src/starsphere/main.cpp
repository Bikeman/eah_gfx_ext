/***************************************************************************
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

#include <cstdlib>
#include <iostream>

#include <diagnostics.h>

#include "WindowManager.h"
#include "ResourceFactory.h"
#include "AbstractGraphicsEngine.h"
#include "GraphicsEngineFactory.h"

#ifdef __APPLE__
#ifdef __cplusplus
extern "C" {
#endif
    void SetMacSSLevel();
#ifdef __cplusplus
}    // extern "C"
#endif
#endif


int main(int argc, char **argv)
{
	// enable BOINC diagnostics
	// TODO: we might want to optimize this for glibc- and mingw-based stacktraces!
	boinc_init_graphics_diagnostics(BOINC_DIAG_DEFAULTS);

	// choose application to be build/used
	GraphicsEngineFactory::Applications scienceApplication;
#ifdef SCIENCE_APP
	scienceApplication = GraphicsEngineFactory::SCIENCE_APP;
#else
	scienceApplication = GraphicsEngineFactory::EinsteinS5R3;
#endif

	// prepare main objects
	WindowManager window;
	ResourceFactory factory;
	AbstractGraphicsEngine *graphics = GraphicsEngineFactory::createInstance(
											GraphicsEngineFactory::Starsphere,
											scienceApplication);

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

	// pepare rendering
	graphics->initialize(window.windowWidth(), window.windowHeight(), fontResource);
	graphics->refreshBOINCInformation();

	// check optional command line parameter
	if(argc == 2) {
		string param(argv[1]);
		if(param == "--fullscreen") {
			// switch to fullscreen (on windoze: after init!)
			window.setScreensaverMode(true);    // Must do this first on Apple
			window.toggleFullscreen();
#ifdef __APPLE__
            SetMacSSLevel();
#endif    
		}
	}

	// enter main event loop
	window.eventLoop();

	// clean up end exit
	window.unregisterEventObserver(graphics);
	delete graphics;
	delete fontResource;

	exit(0);
}
