#include "WindowManager.h"

WindowManager::WindowManager()
{
}

WindowManager::~WindowManager()
{
}

bool WindowManager::initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
		cerr << "Window system could not be initalized: " << SDL_GetError() << endl;
		return false;
	}

	atexit(SDL_Quit);

	// retrieve current video settings
	const SDL_VideoInfo *videoInfo = SDL_GetVideoInfo();

	if (videoInfo->current_w != 0) {
		m_CurrentWidth = m_DesktopWidth = videoInfo->current_w;
	}

	if (videoInfo->current_h != 0) {
		m_CurrentHeight = m_DesktopHeight = videoInfo->current_h;
	}

	if (videoInfo->vfmt->BitsPerPixel != 0) {
		m_DesktopBitsPerPixel = videoInfo->vfmt->BitsPerPixel;
	}

	/*
	 * SDL_SWSURFACE - Das Surface wird im Hauptspeicher abgelegt (default)
	 * SDL_HWSURFACE - Das Surface wird im Grafikspeicher abgelegt
	 * SDL_ASYNCBLIT - Surface benutzt asynchrone Blits, wenn möglich
	 * SDL_ANYFORMAT - Erlaubt jedes Pixel-Format (nur beim display-surface)
	 * SDL_HWPALETTE - Surface nutzt exclusive Farbpalette
	 * SDL_DOUBLEBUF - Surface ist "double buffered" (nur display-surface)
	 * SDL_FULLSCREEN - Surface im Full-Screen-Mode initialisieren (nur display-surface)
	 * SDL_OPENGL - Surface nutzt OpenGL (nur display-surface)
	 * SDL_RESIZABLE - Surfacefenster ist veränderbar (nur display-Surface)
	 * SDL_HWACCEL- Surface blit nutzt Hardwarebeschleunigung
	 * SDL_SRCCOLORKEY - Surface nutzt colorkey blitting
	 * SDL_RLEACCEL - Colorkey blitting ist durch RLE beschleunigt
	 * SDL_SRCALPHA - Surface blit nutzt alpha blending
	 * SDL_PREALLOC - Surface nutzt vorher allokierten Speicher
	 */

	m_VideoModeFlags = SDL_OPENGL | SDL_RESIZABLE;

	Uint32 bitPerPixel = SDL_VideoModeOK(
							m_DesktopWidth,
							m_DesktopHeight,
							m_DesktopBitsPerPixel,
							m_VideoModeFlags);

	if ( !bitPerPixel) {
		cerr << "Video mode not supported: " << SDL_GetError() << endl;
		return false;
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
	//SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	// 4x FSAA, way too heavy on some machines :-)
	// FIXME: without it polygon fonts look really ugly :-(
	// TODO: should be controlled with config values (coupled to disabling text rendering?)
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

	m_DisplaySurface = SDL_SetVideoMode(
							m_DesktopWidth,
							m_DesktopHeight,
							m_DesktopBitsPerPixel,
							m_VideoModeFlags);

	if (m_DisplaySurface == NULL) {
		cerr << "Could not acquire rendering surface: " << SDL_GetError() << endl;
		return false;
	}

	return true;
}

void WindowManager::eventLoop()
{
	SDL_AddTimer(40, &timerCallbackRenderEvent, NULL);
	SDL_AddTimer(1000, &timerCallbackBOINCUpdateEvent, NULL);

	//SDL_EventState( SDL_QUIT, SDL_IGNORE);
	SDL_EventState(SDL_ACTIVEEVENT, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);
	//SDL_EventState( SDL_KEYDOWN, SDL_IGNORE);
	//SDL_EventState( SDL_MOUSEMOTION, SDL_IGNORE);
	SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYAXISMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBALLMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYHATMOTION, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONDOWN, SDL_IGNORE);
	SDL_EventState(SDL_JOYBUTTONUP, SDL_IGNORE);
	//SDL_EventState( SDL_VIDEORESIZE, SDL_IGNORE);
	SDL_EventState(SDL_VIDEOEXPOSE, SDL_IGNORE);
	//SDL_EventState( SDL_USEREVENT, SDL_IGNORE);
	SDL_EventState(SDL_SYSWMEVENT, SDL_IGNORE);

	SDL_Event event;

	static double i = 0.0;

	while (SDL_WaitEvent(&event) ) {
		if (event.type == SDL_USEREVENT && event.user.code == RenderEvent) {
#ifdef DEBUG_VALGRIND
			if(i < 0.25) {
#endif
			graphics->render(i+=0.025);
#ifdef DEBUG_VALGRIND
			}
			else {
				if (m_DisplaySurface) SDL_FreeSurface(m_DisplaySurface);
				exit(0);
			}
#endif      
		}
		else if (event.type == SDL_USEREVENT && event.user.code
		        == BOINCUpdateEvent) {

		}
		else if (event.motion.state & (SDL_BUTTON(1) | SDL_BUTTON(3)) &&
				 event.type == SDL_MOUSEMOTION) {
			
			if (event.motion.state & SDL_BUTTON(1)) {
				graphics->rotateSphere(event.motion.xrel, event.motion.yrel);
			}
			else if (event.motion.state & SDL_BUTTON(3)) {
				graphics->zoomSphere(event.motion.yrel);
			}
		}
		else if (event.type == SDL_VIDEORESIZE) {
			m_CurrentWidth = event.resize.w;
			m_CurrentHeight = event.resize.h;
			
			m_DisplaySurface = SDL_SetVideoMode(
									m_CurrentWidth,
									m_CurrentHeight,
									m_DesktopBitsPerPixel,
									m_VideoModeFlags);
			
			graphics->resize(m_CurrentWidth, m_CurrentHeight);
		}
		else if (event.type == SDL_QUIT ||
				(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
			
			if (m_DisplaySurface) {
				SDL_FreeSurface(m_DisplaySurface);
			}

			exit(0);
		}
		else if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
				case SDLK_s:
					graphics->setFeature(
							graphics->STARS,
					        graphics->isFeature(graphics->STARS) ? false : true);
					break;
				case SDLK_c:
					graphics->setFeature(
					        graphics->CONSTELLATIONS,
					        graphics->isFeature(graphics->CONSTELLATIONS) ? false : true);
					break;
				case SDLK_o:
					graphics->setFeature(
					        graphics->OBSERVATORIES,
					        graphics->isFeature(graphics->OBSERVATORIES) ? false : true);
					break;
				case SDLK_x:
					graphics->setFeature(
							graphics->XRAYS,
					        graphics->isFeature(graphics->XRAYS) ? false : true);
					break;
				case SDLK_p:
					graphics->setFeature(
							graphics->PULSARS,
					        graphics->isFeature(graphics->PULSARS) ? false : true);
					break;
				case SDLK_r:
					graphics->setFeature(
							graphics->SNRS,
					        graphics->isFeature(graphics->SNRS) ? false : true);
					break;
				case SDLK_g:
					graphics->setFeature(
							graphics->GLOBE,
					        graphics->isFeature(graphics->GLOBE) ? false : true);
					break;
				case SDLK_a:
					graphics->setFeature(
							graphics->AXES,
					        graphics->isFeature(graphics->AXES) ? false : true);
					break;
				case SDLK_i:
					graphics->setFeature(
							graphics->SEARCHINFO,
					        graphics->isFeature(graphics->SEARCHINFO) ? false : true);
					break;
				case SDLK_l:
					graphics->setFeature(
							graphics->LOGO,
					        graphics->isFeature(graphics->LOGO) ? false : true);
					break;
				case SDLK_RETURN:
					toggleFullscreen();
				default:
					break;
			}
		}
	}
}

void WindowManager::registerEventObserver()
{

}

void WindowManager::unregisterEventObserver()
{

}

Uint32 WindowManager::timerCallbackRenderEvent(Uint32 interval, void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = RenderEvent;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);

	return interval;
}

Uint32 WindowManager::timerCallbackBOINCUpdateEvent(Uint32 interval,
        void *param)
{
	SDL_Event event;
	SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = BOINCUpdateEvent;
	userevent.data1 = NULL;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);

	return interval;
}

int WindowManager::windowWidth() const
{
	return m_CurrentWidth;
}

int WindowManager::windowHeight() const
{
	return m_CurrentHeight;
}

void WindowManager::setWindowCaption(const string caption)
{
	SDL_WM_SetCaption(caption.c_str(), NULL);
}

void WindowManager::setWindowIcon(const string filename)
{
	if (filename.length() > 0) {
		SDL_WM_SetIcon(SDL_LoadBMP(filename.c_str()), NULL);
	}
}

void WindowManager::toggleFullscreen()
{
	SDL_WM_ToggleFullScreen(m_DisplaySurface);
	SDL_ShowCursor(SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE ? SDL_DISABLE : SDL_ENABLE);
}

void WindowManager::setRenderEngine(Starsphere *graphics)
{
	this->graphics = graphics;
}
