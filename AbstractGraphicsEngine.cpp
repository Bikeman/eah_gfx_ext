#include "AbstractGraphicsEngine.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() : boincAdapter() 
{
}

AbstractGraphicsEngine::~AbstractGraphicsEngine()
{
}

void AbstractGraphicsEngine::refreshBOINCInformation()
{
	boincAdapter.refresh();
}
