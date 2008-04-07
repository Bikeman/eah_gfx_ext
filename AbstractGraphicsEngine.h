#ifndef ABSTRACTGRAPHICSENGINE_H_
#define ABSTRACTGRAPHICSENGINE_H_

#include "BOINCClientAdapter.h"
#include "Resource.h"

class AbstractGraphicsEngine
{
public:
	virtual ~AbstractGraphicsEngine();
	
	virtual void initialize(const int width, const int height, const Resource *font) = 0;
	virtual void resize(const int width, const int height) = 0;
	virtual void render(const double timeOfDay) = 0;
	
	virtual void mouseButtonEvent(const int positionX, const int positionY, const int buttonPressed) = 0;
	virtual void mouseMoveEvent(const int deltaX, const int deltaY, const int buttonPressed) = 0;
	virtual void keyboardPressEvent(const int keyPressed) = 0;
	
	virtual void refreshBOINCInformation();
	
	enum MouseButton {
		MouseButtonLeft = 1,
		MouseButtonRight = 2
	};
	
	enum KeyBoardKey {
		KeyA = 0x1,
		KeyB = 0x2,
		KeyC = 0x4,
		KeyD = 0x8,
		KeyE = 0x10,
		KeyF = 0x20,
		KeyG = 0x40,
		KeyH = 0x80,
		KeyI = 0x100,
		KeyJ = 0x200,
		KeyK = 0x400,
		KeyL = 0x800,
		KeyM = 0x1000,
		KeyN = 0x2000,
		KeyO = 0x4000,
		KeyP = 0x8000,
		KeyQ = 0x10000,
		KeyR = 0x20000,
		KeyS = 0x40000,
		KeyT = 0x80000,
		KeyU = 0x100000,
		KeyV = 0x200000,
		KeyW = 0x400000,
		KeyX = 0x800000,
		KeyY = 0x1000000,
		KeyZ = 0x2000000,
		KeyEnter = 0x4000000,
		KeyEscape = 0x8000000
	};

protected:
	AbstractGraphicsEngine();
	
	BOINCClientAdapter boincAdapter;
};

#endif /*ABSTRACTGRAPHICSENGINE_H_*/
