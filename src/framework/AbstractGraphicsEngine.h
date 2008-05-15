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

#ifndef ABSTRACTGRAPHICSENGINE_H_
#define ABSTRACTGRAPHICSENGINE_H_

#include "BOINCClientAdapter.h"
#include "Resource.h"

/**
 * \brief This abstract class provides common features for all graphics engines
 * 
 * All graphics engines (Open GL visualisation code) have to be derived from this class.
 * First of all it defines the common interface which the rest of the graphics framework
 * expects any implementing classes to support. Apart from that, this class also defines
 * common properties like event idetifiers and basic access to BOINC client information.
 *  
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class AbstractGraphicsEngine
{
public:
	/// Destructor
	virtual ~AbstractGraphicsEngine();
	
	/**
	 * \brief This method is called when an implementing graphics engine should initialize itself
	 * 
	 * \param width The current width of the display surface
	 * \param height The current height of the display surface
	 * \param font A pointer to a Resource object containing TTF font faces for text rendering 
	 * \param recycle This flag indicates whether we initialize (FALSE) or reinitialize (TRUE) the context
	 */
	virtual void initialize(const int width, const int height, const Resource *font, const bool recycle = false) = 0;
	
	/**
	 * \brief This method is called when the windowing system encounters a window resize event
	 * 
	 * \param width The new width of the display surface
	 * \param height The new height of the display surface
	 */
	virtual void resize(const int width, const int height) = 0;
	
	/**
	 * \brief This method is called when an implementing graphics engine should render one frame
	 * 
	 * \param timeOfDay The current time in "seconds since the Epoch" (with microsecond precision)
	 */	
	virtual void render(const double timeOfDay) = 0;
	
	/**
	 * \brief Defined mouse button identifiers
	 * 
	 * \see mouseButtonEvent
	 * \see mouseMoveEvent
	 */ 	
	enum MouseButton {
		MouseButtonLeft = 1,
		MouseButtonRight = 2
	};
	
	/**
	 * \brief This method is called when the windowing system encounters a mouse button event
	 * 
	 * \param positionX The mouse position on the x-axis when the event occurred (range: 0-width)
	 * \param positionY The mouse position on the y-axis when the event occurred (range: 0-height)
	 * \param buttonPressed The mouse button pressed (if any) when the event occurred.
	 * It can be identified using the elements of \ref MouseButton.
	 * 
	 * \see MouseButton
	 */	
	virtual void mouseButtonEvent(const int positionX, const int positionY, const MouseButton buttonPressed) = 0;
	
	/**
	 * \brief This method is called when the windowing system encounters a mouse move event
	 * 
	 * \param deltaX The relative mouse position change with respect to the x-axis when the event occurred
	 * \param deltaY The relative mouse position change with respect to the y-axis when the event occurred
	 * \param buttonPressed The mouse button pressed (if any) when the event occurred.
	 * It can be identified using the elements of \ref MouseButton.
	 *
	 * \see MouseButton
	 */	
	virtual void mouseMoveEvent(const int deltaX, const int deltaY, const MouseButton buttonPressed) = 0;
	
	/**
	 * \brief Defined keyboard identifiers
	 * 
	 * \see keyboardPressEvent
	 */ 
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
	
	/**
	 * \brief This method is called when the windowing system encounters a key press event
	 * 
	 * \attention Please note that not all key events are currently forwarded (this should be change
	 * as soon as the need arises). Please see WindowManager::eventLoop for details.
	 * 
	 * \param keyPressed The keyboard key pressed. It can be identified using the elements of \ref KeyBoardKey.
	 * 
	 * \see KeyBoardKey
	 * \see WindowManager::eventLoop
	 */
	virtual void keyboardPressEvent(const KeyBoardKey keyPressed) = 0;
	
	/**
	 * \brief This method is called when the BOINC client information should be updated
	 * 
	 * When you inherit from this class and implement this method, please make sure you call
	 * \ref refreshLocalBOINCInformation() to invoke the generic default implementation which
	 * refreshes \ref m_BoincAdapter.
	 * 
	 * \see refreshLocalBOINCInformation()
	 */
	virtual void refreshBOINCInformation() = 0;

protected:
	/**
	 * \brief Default constructor
	 * 
	 * The constructor is protected since this is an abstract class.
	 */
	AbstractGraphicsEngine();

	/**
	 * \brief This method has to be called in order to update the BOINC client information
	 * 
	 * This is the local/generic implementation which refreshes \ref m_BoincAdapter.
	 * 
	 * \see refreshBOINCInformation()
	 */
	virtual void refreshLocalBOINCInformation();
	
	/// BOINC client adapter instance for information retrieval
	BOINCClientAdapter m_BoincAdapter;
};

#endif /*ABSTRACTGRAPHICSENGINE_H_*/
