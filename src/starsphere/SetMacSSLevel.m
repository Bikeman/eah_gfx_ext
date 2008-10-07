/***************************************************************************
 *   Copyright (C) 2008 by Charlie Fenton for Oliver Bock                  *
 *   contact: oliver.bock[AT]aei.mpg.de                                    *
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


#include <Cocoa/Cocoa.h>

void SetMacSSLevel();

// The standard ScreenSaverView class actually sets the window 
// level to 2002, not the 1000 defined by NSScreenSaverWindowLevel 
// and kCGScreenSaverWindowLevel
#define RealSaverLevel 2002

void SetMacSSLevel() {
    NSOpenGLContext * myContext = nil;
    NSView *myView = nil;
    NSWindow* myWindow = nil;

    // In screensaver mode, set our window's level just above 
    // our BOINC screensaver's window level so it can appear 
    // over it.  This doesn't interfere with the screensaver 
    // password dialog because the dialog appears only after 
    // our screensaver is closed.
    myContext = [ NSOpenGLContext currentContext ];
    if (myContext)
        myView = [ myContext view ];
    if (myView)
        myWindow = [ myView window ];
    if (myWindow == nil)
        return;
        
    [ myWindow setLevel:RealSaverLevel+20 ];
    [ myWindow setFrameOrigin:NSMakePoint(0.0, 0.0) ];
}
