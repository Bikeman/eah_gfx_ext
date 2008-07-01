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

#include "WindowManager.h"
#include "ResourceFactory.h"
#include "AbstractGraphicsEngine.h"
#include "GraphicsEngineFactory.h"


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

	// pepare rendering
	graphics->initialize(window.windowWidth(), window.windowHeight(), fontResource);
	graphics->refreshBOINCInformation();
	
	// check optional command line parameter
	if(argc == 2) {
		string param(argv[1]);
		if(param == "--fullscreen") {
			// switch to fullscreen (on windoze: after init!)
			window.toggleFullscreen();
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
