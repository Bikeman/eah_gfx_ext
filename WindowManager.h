#ifndef WINDOWMANAGER_H_
#define WINDOWMANAGER_H_

#include <iostream>
#include <string>
#include <list>
#include <cassert>

#include <SDL.h>

#include "AbstractGraphicsEngine.h"

using namespace std;

class WindowManager
{
public:
	WindowManager();
	virtual ~WindowManager();
	
	bool initialize();
	void registerEventObserver(AbstractGraphicsEngine *engine);
	void unregisterEventObserver(AbstractGraphicsEngine *engine);
	void eventLoop();
	
	int windowWidth() const;
	int windowHeight() const;
	
	void setWindowCaption(const string caption);
	void setWindowIcon(const string filename);
	void toggleFullscreen();

private:
    // FIXME: work around static, otherwise event conflict when more than one instance
    static Uint32 timerCallbackRenderEvent( Uint32 interval, void *param );
    static Uint32 timerCallbackBOINCUpdateEvent( Uint32 interval, void *param );
    
    int m_DesktopWidth;
    int m_DesktopHeight;
    int m_DesktopBitsPerPixel;
    int m_CurrentWidth;
    int m_CurrentHeight;
    Uint32 m_VideoModeFlags;
    SDL_Surface *m_DisplaySurface;
    
    enum EventCodes {
        RenderEvent,
        BOINCUpdateEvent
    };
    
    list<AbstractGraphicsEngine *> eventObservers;
};

#endif /*WINDOWMANAGER_H_*/
