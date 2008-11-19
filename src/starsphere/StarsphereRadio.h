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

#ifndef STARSPHERERADIO_H_
#define STARSPHERERADIO_H_

#include <sstream>
#include <vector>

#include "Starsphere.h"
#include "EinsteinRadioAdapter.h"

//FIXME: these can be removed after FFT testing!
#include <cstdlib>
#include <ctime>

using namespace std;

/**
 * \addtogroup starsphere Starsphere
 * @{
 */

/**
 * \brief Specialized rendering engine for the "Arecibo Radio Pulsar Search" science run
 *
 * This class comprises the specialized parts of the Starsphere rendering engine.
 * The main differences stem from the fact that most science runs differ in their
 * search configuration and parameters. Thus the parameters exposed by the HUD
 * (head-up display) are positioned and rendered here. For the time being the
 * "BOINC Information" are top-aligned to the "Search Parameters", hence they're
 * also positioned and rendered here. This specific implementation also introduces a
 * nice feature that visualizes the \a actual search data in guise of a \a real-time power
 * spectrum.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class StarsphereRadio : public Starsphere
{
public:
	/// Default contructor
	StarsphereRadio();

	/// Destructor
	virtual ~StarsphereRadio();

	/**
	 * \brief This method is called to initialize the engine
	 *
	 * As this method overrides its parent's implementation, it calls Starsphere::initialize()
	 * first in order to "add" the specialized parts afterwards.
	 *
	 * \param width The current width of the display surface
	 * \param height The current height of the display surface
	 * \param font A pointer to a Resource object containing TTF font faces for text rendering
	 * \param recycle This flag indicates whether we initialize (FALSE) or reinitialize (TRUE) the context
	 */
	virtual void initialize(const int width, const int height, const Resource *font, const bool recycle = false);

	/**
	 * \brief This method is called when the windowing system encounters a window resize event
	 *
	 * As this method overrides its parent's implementation, it calls Starsphere::resize()
	 * first in order to "add" the specialized parts afterwards.
	 *
	 * \param width The new width of the display surface
	 * \param height The new height of the display surface
	 */
	void resize(const int width, const int height);

	/**
	 * \brief This method is called when the BOINC client information should be updated
	 *
	 * This method implements AbstractGraphicsEngine::refreshBOINCInformation() and calls
	 * Starsphere::refreshLocalBOINCInformation() first and "adds" the specialized
	 * parts afterwards.
	 *
	 * \see AbstractGraphicsEngine::refreshBOINCInformation()
	 * \see Starsphere::refreshLocalBOINCInformation()
	 */
	void refreshBOINCInformation();

private:
	/**
	 * \brief Render science run specific logo
	 *
	 * This specific implementation shows the usual "Einstein@Home" logo combined
	 * with "International Year of Astronomy 2009" as subtitle
	 */
	inline void renderLogo();

	/**
	 * \brief Render science run specific search information
	 *
	 * For this specific implementation this also includes the "BOINC Statistics"
	 * as it is top-aligned to the "Search Information".
	 */
	inline void renderSearchInformation();

	/**
	 * \brief Renders the Arecibo observatory
	 *
	 * This specific implementation invokes a single display list rendering the
	 * Arecibo observatory. It overrides the (empty) base class implementation.
	 */
	inline void renderAdditionalObservatories();

	/**
	 * \brief Creates an OpenGL call list which contains the static power spectrum coordinate system
	 *
	 * \param originX The x-screen coordinate of the power spectrum's origin
	 * \param originY The y-screen coordinate of the power spectrum's origin
	 */
	void generatePowerSpectrumCoordSystem(const int originX, const int originY);

	/**
	 * \brief Creates an OpenGL call list which contains the dynamic power spectrum frequency bins
	 *
	 * \param originX The x-screen coordinate of the power spectrum's origin
	 * \param originY The y-screen coordinate of the power spectrum's origin
	 */
	void generatePowerSpectrumBins(const int originX, const int originY);

	/**
	 * \brief Generates the OpenGL call lists for the displayed observatories
	 *
	 * \param dimFactor A dim factor (range: 0 <= x <= 1) that will, well, dim the color
	 * of the observatories. Right now the factor is propagated to the base class
	 * implementation, hence dims the IFOs. The local Arecibo observatory is currently
	 * unaffected.
	 */
	void generateObservatories(const float dimFactor);

	/// ID of the OpenGL call list which contains the static power spectrum coordinate system
	GLuint m_PowerSpectrumCoordSystemList;

	/// ID of the OpenGL call list which contains the dynamic power spectrum frequency bins
	GLuint m_PowerSpectrumBinList;

	/// Byte vector to hold the current power spectrum bin values
	const vector<unsigned char>* m_PowerSpectrumFreqBins;

	/// Power Spectrum configuration setting (width)
	GLfloat m_PowerSpectrumWidth;

	/// Power Spectrum configuration setting (height)
	GLfloat m_PowerSpectrumHeight;

	/// Power Spectrum configuration setting (horizontal position of the origin relative to the window width)
	GLfloat m_PowerSpectrumOriginWidthOffset;

	/// Power Spectrum configuration setting (vertical position of the origin relative to the window height)
	GLfloat m_PowerSpectrumOriginHeightOffset;

	/// Power Spectrum configuration setting (horizontal position)
	GLfloat m_PowerSpectrumXPos;

	/// Power Spectrum configuration setting (vertical position)
	GLfloat m_PowerSpectrumYPos;

	/// Power Spectrum configuration setting (axes width)
	GLfloat m_PowerSpectrumAxesWidth;

	/// Power Spectrum configuration setting (bin width)
	GLfloat m_PowerSpectrumBinWidth;

	/// Power Spectrum configuration setting (bin distance)
	GLfloat m_PowerSpectrumBinDistance;

	/// Power Spectrum label configuration setting (horizontal position)
	GLfloat m_PowerSpectrumLabelXPos;

	/// Power Spectrum label configuration setting (vertical position)
	GLfloat m_PowerSpectrumLabelYPos;

	/// Power Spectrum label configuration setting (horizontal position relative to the power spectrum origin)
	GLfloat m_PowerSpectrumLabelXOffset;

	/// Power Spectrum label configuration setting (vertical position relative to the power spectrum origin)
	GLfloat m_PowerSpectrumLabelYOffset;

	/// Active render quality setting
	BOINCClientAdapter::GraphicsQualitySetting m_QualitySetting;

	/// Specialized BOINC client adapter instance for information retrieval
	EinsteinRadioAdapter m_EinsteinAdapter;

	/// Formatted string copy of the current WU's search parameter "Right-Ascension" (degrees)
	string m_WUSkyPosRightAscension;

	/// Formatted string copy of the current WU's search parameter "Declination" (degrees)
	string m_WUSkyPosDeclination;

	/// Formatted string copy of the current WU's search parameter "Dispersion measure"
	string m_WUDispersionMeasure;

	/// Local value copy of the current WU's search parameter "Dispersion measure"
	double m_WUDispersionMeasureValue;

	/// Formatted string copy of the current template's search parameter "Projected orbital radius"
	string m_WUTemplateOrbitalRadius;

	/// Formatted string copy of the current template's search parameter "Orbital period"
	string m_WUTemplateOrbitalPeriod;

	/// Formatted string copy of the current template's search parameter "Initial orbital phase"
	string m_WUTemplateOrbitalPhase;

	/// Formatted string copy of the current WU's search parameter "Percent done"
	string m_WUPercentDone;

	/// Formatted string copy of the current WU's search parameter "CPU Time"
	string m_WUCPUTime;

	/// HUD configuration setting (line offset for medium sized font)
	GLfloat m_YOffsetMedium;

	/// HUD configuration setting (horizontal start position for the right part)
	GLfloat m_XStartPosRight;

	/// HUD configuration setting (horizontal start position for the right part relative to the window width)
	GLfloat m_XStartPosRightWidthOffset;

	/// HUD configuration setting (vertical start position for the bottom part, header)
	GLfloat m_YStartPosBottom;

	/// HUD configuration setting (vertical start position for the bottom part, line 1)
	GLfloat m_Y1StartPosBottom;

	/// HUD configuration setting (vertical start position for the bottom part, line 2)
	GLfloat m_Y2StartPosBottom;

	/// HUD configuration setting (vertical start position for the bottom part, line 3)
	GLfloat m_Y3StartPosBottom;

	/// HUD configuration setting (vertical start position for the bottom part, line 4)
	GLfloat m_Y4StartPosBottom;

	/// HUD configuration setting (vertical start position for the bottom part, line 5)
	GLfloat m_Y5StartPosBottom;

	/// HUD configuration setting (vertical start position for the bottom part, line 6)
	GLfloat m_Y6StartPosBottom;

	/// Arecibo Observatory display list
	GLuint m_areciboObservatory;
};

/**
 * @}
 */

#endif /*STARSPHERES5R3_H_*/
