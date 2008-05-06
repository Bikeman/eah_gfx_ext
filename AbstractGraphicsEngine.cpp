#include "AbstractGraphicsEngine.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() : m_BoincAdapter() 
{
}

AbstractGraphicsEngine::~AbstractGraphicsEngine()
{
}

void AbstractGraphicsEngine::refreshLocalBOINCInformation()
{
	m_BoincAdapter.refresh();
}
