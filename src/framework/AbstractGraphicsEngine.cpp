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

#include "AbstractGraphicsEngine.h"

AbstractGraphicsEngine::AbstractGraphicsEngine() : m_BoincAdapter() 
{
	m_InitialBOINCRefreshDone = false;
}

AbstractGraphicsEngine::~AbstractGraphicsEngine()
{
}

int AbstractGraphicsEngine::initialWindowWidth()
{
	if(!m_InitialBOINCRefreshDone) {
		m_BoincAdapter.refresh();
		m_InitialBOINCRefreshDone = true;
	}
	
	return m_BoincAdapter.graphicsWindowWidth();
}

int AbstractGraphicsEngine::initialWindowHeight()
{
	if(!m_InitialBOINCRefreshDone) {
		m_BoincAdapter.refresh();
		m_InitialBOINCRefreshDone = true;
	}
	
	return m_BoincAdapter.graphicsWindowHeight();
}

int AbstractGraphicsEngine::frameRate()
{
	if(!m_InitialBOINCRefreshDone) {
		m_BoincAdapter.refresh();
		m_InitialBOINCRefreshDone = true;
	}
	
	return m_BoincAdapter.graphicsFrameRate();
}

void AbstractGraphicsEngine::refreshLocalBOINCInformation()
{
	m_BoincAdapter.refresh();
}
