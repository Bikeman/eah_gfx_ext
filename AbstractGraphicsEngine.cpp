#include "AbstractGraphicsEngine.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() : m_BoincAdapter() 
{
}

AbstractGraphicsEngine::~AbstractGraphicsEngine()
{
}

void AbstractGraphicsEngine::refreshBOINCInformation()
{
	m_BoincAdapter.refresh();
}
