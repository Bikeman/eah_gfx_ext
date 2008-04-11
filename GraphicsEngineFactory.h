#ifndef GRAPHICSENGINEFACTORY_H_
#define GRAPHICSENGINEFACTORY_H_

#include "AbstractGraphicsEngine.h"
#include "StarsphereS5R3.h"

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
	virtual ~GraphicsEngineFactory();
	
	/// Identifiers of supported graphics engines
	enum Engines {
		Starsphere = 1
	};
	
	/// Identifiers of supported science applications
	enum Applications {
		EinsteinS5R3 = 53
	};
	
	/**
	 * \brief Instaniates a new graphics engine
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
	// Contructor (private since this a purely static factory)
	GraphicsEngineFactory();
};

#endif /*GRAPHICSENGINEFACTORY_H_*/
