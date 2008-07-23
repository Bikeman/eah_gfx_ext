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

#ifndef GRAPHICSENGINEFACTORY_H_
#define GRAPHICSENGINEFACTORY_H_

#include "AbstractGraphicsEngine.h"
#include "StarsphereS5R3.h"
#include "StarsphereRadio.h"

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief Factory to create graphics engines
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class GraphicsEngineFactory
{
public:
	/// Destructor
	virtual ~GraphicsEngineFactory();

	/// Identifiers of supported graphics engines
	enum Engines {
		Starsphere = 1
	};

	/// Identifiers of supported science applications
	enum Applications {
		EinsteinS5R3 = 53,
		EinsteinRadio = 42
	};

	/**
	 * \brief Instantiates a new graphics engine
	 *
	 * Use this method to create a new grahics engine instance. However, please make
	 * that you use only sensible combinations of \c engine and \c application (you
	 * should know them).
	 *
	 * \param engine The identifier of the requested graphics engine
	 * \param application The identifier of the requested science application support
	 *
	 * \return The pointer to the new engine instance
	 *
	 * \see Engines
	 * \see Applications
	 */
	static AbstractGraphicsEngine * createInstance(Engines engine, Applications application);

private:
	/// Contructor (private since this a purely static factory)
	GraphicsEngineFactory();
};

/**
 * @}
 */

#endif /*GRAPHICSENGINEFACTORY_H_*/
