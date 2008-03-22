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
#include <SDL/SDL.h>

#include "starsphere.h" 

// ugly globals, will eventually factored out into private members
SDL_Surface *m_DisplaySurface = 0;
int desktopWidth = 800;
int desktopHeight = 600;
int desktopBitsPerPixel = 16;


using namespace std;

Uint32 timerCallback( Uint32 interval, void *param )
{
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes an SDL_USEREVENT event
       into the queue, and causes ourself to be called again at the
       same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent( &event );
    
    return( interval );
}


void eventLoop()
{
	SDL_AddTimer(40, timerCallback, NULL);
	
	//SDL_EventState( SDL_QUIT, SDL_IGNORE);
	SDL_EventState( SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState( SDL_KEYUP, SDL_IGNORE);
	//SDL_EventState( SDL_KEYDOWN, SDL_IGNORE);
	SDL_EventState( SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState( SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState( SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState( SDL_JOYBALLMOTION, SDL_IGNORE);
	SDL_EventState( SDL_JOYHATMOTION, SDL_IGNORE);
	SDL_EventState( SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState( SDL_JOYBUTTONUP, SDL_IGNORE);
	SDL_EventState( SDL_VIDEORESIZE, SDL_IGNORE);
	SDL_EventState( SDL_VIDEOEXPOSE, SDL_IGNORE);
	//SDL_EventState( SDL_USEREVENT, SDL_IGNORE);
	SDL_EventState( SDL_SYSWMEVENT, SDL_IGNORE);

	SDL_Event event;
	
	static double i = 0.0;

	while ( SDL_WaitEvent(&event) )
	{
		if( event.type == SDL_USEREVENT )
		{
			app_graphics_render(desktopWidth, desktopHeight, i+=0.025);
		}
		else if( event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) )
		{
			exit(0);
		}
		else if( event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN )
		{
			SDL_WM_ToggleFullScreen( m_DisplaySurface );
			app_graphics_render(desktopWidth, desktopHeight, i);
		}
	}
}


int main(int argc, char **argv) {
	
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		throw runtime_error( SDL_GetError() );
	}

    // retrieve current video settings
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();
	
	if (videoInfo->current_w != 0) {
		desktopWidth = videoInfo->current_w;
	}

	if (videoInfo->current_h != 0) {
		desktopHeight = videoInfo->current_h;
	}
	
	if (videoInfo->vfmt->BitsPerPixel != 0) {
		desktopBitsPerPixel = videoInfo->vfmt->BitsPerPixel;
	}
	
#ifdef DEBUG
	cout << "Using resolution: " << desktopWidth << "/" << desktopHeight << "/" << desktopBitsPerPixel << endl;
#endif

	/*
	 * SDL_SWSURFACE - Das Surface wird im Hauptspeicher abgelegt (default)
	 * SDL_HWSURFACE - Das Surface wird im Grafikspeicher abgelegt
	 * SDL_ASYNCBLIT - Surface benutzt asynchrone Blits, wenn möglich
	 * SDL_ANYFORMAT - Erlaubt jedes Pixel-Format (nur beim display-surface)
	 * SDL_HWPALETTE - Surface nutzt exclusive Farbpalette
	 * SDL_DOUBLEBUF - Surface ist "double buffered" (nur display-surface)
	 * SDL_FULLSCREEN - Surface im Full-Screen-Mode initialisieren (nur display-surface)
	 * SDL_OPENGL - Surface nutzt OpenGL (nur display-surface)
	 * SDL_OPENGLBLIT - Surface unterstützt OpenGL blitting (nur display-surface)
	 * SDL_RESIZABLE - Surfacefenster ist veränderbar (nur display-Surface)
	 * SDL_HWACCEL- Surface blit nutzt Hardwarebeschleunigung
	 * SDL_SRCCOLORKEY - Surface nutzt colorkey blitting
	 * SDL_RLEACCEL - Colorkey blitting ist durch RLE beschleunigt
	 * SDL_SRCALPHA - Surface blit nutzt alpha blending
	 * SDL_PREALLOC - Surface nutzt vorher allokierten Speicher
	 */

	Uint32 bitPerPixel = SDL_VideoModeOK( desktopWidth, desktopHeight, desktopBitsPerPixel, SDL_OPENGL);

	if ( !bitPerPixel) {
		cerr << "Video mode not supported: " << SDL_GetError() << endl;
		exit(1);
	}

	// specify minimum requirements
	// (query with SDL_GL_SetAttribute() after SDL_SetVideoMode() if needed)
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 1);
	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	// 4x FSAA :-)
	SDL_GL_SetAttribute (SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute (SDL_GL_MULTISAMPLEBUFFERS,1);

	m_DisplaySurface = SDL_SetVideoMode( desktopWidth, desktopHeight, desktopBitsPerPixel, SDL_OPENGL);

	if (m_DisplaySurface == NULL) {
		cerr << "Could not acquire rendering surface: " << SDL_GetError() << endl;
		exit(1);
	}

	SDL_WM_SetCaption("Einstein@Home", "Icon");
	//SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL); 
	
	SDL_WM_ToggleFullScreen( m_DisplaySurface );

	app_graphics_resize(desktopWidth, desktopHeight);
	app_graphics_init();
	app_graphics_render(desktopWidth, desktopHeight, 22);
	
	eventLoop();

	return(0);
}
