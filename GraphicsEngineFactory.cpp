#include "GraphicsEngineFactory.h"

GraphicsEngineFactory::~GraphicsEngineFactory()
{
}
GraphicsEngineFactory::GraphicsEngineFactory()
{
}

AbstractGraphicsEngine * GraphicsEngineFactory::createInstance(
							GraphicsEngineFactory::Engines engine,
							GraphicsEngineFactory::Applications application)
{
	switch(engine) {
		case Starsphere:
			switch(application) {
				case EinsteinS5R3:
					return new StarsphereS5R3();
					break;
				default:
					return NULL;
			}
			break;
		default:
			return NULL;	
	}
}
