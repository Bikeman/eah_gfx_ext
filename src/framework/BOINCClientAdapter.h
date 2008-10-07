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

#ifndef BOINCCLIENTADAPTER_H_
#define BOINCCLIENTADAPTER_H_

#include <iostream>
#include <string>
#include <sstream>

#include "boinc_api.h"
#include "graphics2.h"

#include "XMLProcessorInterface.h"

using namespace std;

/**
 * \addtogroup framework Framework
 * @{
 */

/**
 * \brief Adapter class which facilitates communications with the BOINC client
 *
 * This adapter class can be used to query the BOINC core client for information
 * about the user and the running science application instance.
 *
 * \author Oliver Bock\n
 * Max-Planck-Institute for Gravitational Physics\n
 * Hannover, Germany
 */
class BOINCClientAdapter
{
public:
	/**
	 * \brief Constructor
	 *
	 * It takes as an argument the name of the shared memory area to be used
	 * for inter-process communication.
	 *
	 * \param sharedMemoryAreaIdentifier The identifier of the shared memory area
	 */
	BOINCClientAdapter(string sharedMemoryAreaIdentifier);

	/// Destructor
	virtual ~BOINCClientAdapter();

	/**
	 * \brief Defined quality settings for graphics applications
	 *
	 * \see graphicsQualitySetting
	 * \see graphicsFrameRate
	 */
	enum GraphicsQualitySetting {
		LowGraphicsQualitySetting = 1,
		MediumGraphicsQualitySetting = 2,
		HighGraphicsQualitySetting = 4
	};

	/**
	 * \brief Initializes the BOINC client adapter instance
	 *
	 * This method has to be called first, otherwise no data will be returned when requested!
	 */
	void initialize();

	/**
	 * \brief Refreshes dynamic data (e.g. search information)
	 *
	 * You want to call this method periodically to refresh any volatile client information.
	 * Please make sure that you call initialize() first!
	 *
	 * \see AbstractGraphicsEngine::refreshBOINCInformation
	 * \see initialize
	 */
	void refresh();

	/**
	 * \brief Retrieves the BOINC core client version of the currently active client
	 *
	 * \return The BOINC core client version
	 */
	string coreVersion() const;

	/**
	 * \brief Retrieves the \b Einstein\@Home application name of the currently active work unit
	 *
	 * \return The \b Einstein\@Home application name
	 */
	string applicationName() const;

	/**
	 * \brief Retrieves the \b Einstein\@Home application version of the currently active work unit
	 *
	 * \return The \b Einstein\@Home application version
	 */
	string applicationVersion() const;

	/**
	 * \brief Retrieves the BOINC user name currently logged in
	 *
	 * \return The BOINC user name
	 */
	string userName() const;


	/**
	 * \brief Retrieves the BOINC team name of the currently logged in user
	 *
	 * \return The BOINC team name
	 */
	string teamName() const;

	/**
	 * \brief Retrieves the total project credit of the currently logged in user
	 *
	 * \return The total project credit
	 */
	double userCredit() const;

	/**
	 * \brief Retrieves the recent average project credit (RAC) of the currently logged in user
	 *
	 * \return The recent average project credit
	 */
	double userRACredit() const;

	/**
	 * \brief Retrieves the total project credit of this host
	 *
	 * \return The total project credit
	 */
	double hostCredit() const;

	/**
	 * \brief Retrieves the recent average project credit (RAC) of this host
	 *
	 * \return The recent average project credit
	 */
	double hostRACredit() const;

	/**
	 * \brief Retrieves the name of the currently active work unit
	 *
	 * \return The work unit name
	 */
	string wuName() const;

	/**
	 * \brief Retrieves the estimated number of floating-point operations to complete the currently active work unit.
	 *
	 * An estimate of the average number of floating-point operations (FPOPS) required to complete the computation.
	 * This is used to estimate how long the computation will take on a given host.
	 *
	 * \return The estimated number of FPOPS to complete the currently active work unit
	 */
    double wuFPOpsEstimated() const;

	/**
	 * \brief Retrieves the maximum number of floating-point operations allowed for the currently active work unit.
	 *
	 * A bound on the number of floating-point operations (FPOPS) required to complete the computation.
	 * If this bound is exceeded, the application will be aborted.
	 *
	 * \return The maximum number of FPOPS allowed for the currently active work unit
	 */
    double wuFPOpsBound() const;

	/**
	 * \brief Retrieves an estimate of application's largest working set size.
	 *
	 * An estimate of application's largest working set size.
	 * The workunit will only be sent to hosts with at least this much available RAM.
	 *
	 * \return The estimate of application's largest working set size
	 */
    double wuMemoryBound() const;

	/**
	 * \brief Retrieves the maximum disk space allowed to be used by the application.
	 *
	 * A bound on the maximum disk space used by the application, including all input, temporary, and output files.
	 * The workunit will only be sent to hosts with at least this much available disk space.
	 * If this bound is exceeded, the application will be aborted.
	 *
	 * \return The maximum disk space allowed to be used by the application
	 */
    double wuDiskBound() const;

	/**
	 * \brief Retrieves the accumulated time spent on the currently active work unit
	 *
	 * \return The time spent on the work unit
	 */
    double wuCPUTimeSpent() const;

    /**
     * \brief Retrieves information provided by the running science application
     *
     * \return The application specific information string (i.e. XML) found in APP_INIT_DATA
     *
     * \see m_UserData
     */
    string applicationInformation() const;

    /**
     * \brief Retrieves specific information provided by the currently active project
     *
	 * All projects using this framework are highly recommended to adhere to the following XML schema
	 * (not yet literally a XML schema, will be provided later) with respect to graphics settings:
     *
     \verbatim
     <project_preferences>
     	<graphics fps="20" quality="low" width="800" height="600">
     		<engine name="starsphere">
     			<feature id="globe" enabled="true" />
     		</engine>
     		<engine name="waverider">
     			<feature id="sound" enabled="false" />
     		</engine>
     	</graphics>
     </project_preferences>
     \endverbatim
     *
     * The \c graphics tag and its four attributes \b must be provided as shown above where
     * the \c fps attribute contains the frame rate as integer value and \c quality contains
     * a lowercase string value describing the quality setting to be used (supported values: \c low,
     * \c medium, \c high). The two remaining attributes \c width and \c height determine the initial
     * window size when starting in windowed mode. Please note that the children of the \c graphics
     * tag are recommendations of how graphics application specific settings (e.g. features) should be
     * stored per implementation.
	 *
     * \return The project specific information string (i.e. XML) found in \c APP_INIT_DATA
     *
     * \see m_UserData
     * \see GraphicsQualitySetting
     * \see graphicsFrameRate
     * \see graphicsQualitySetting
     */
    string projectInformation() const;

    /**
     * \brief Retrieves the frame rate at which the project's graphics application should be rendered
     *
     * This setting is given by the \c fps attribute of the \c graphics tag that's
     * part of the \c project_specific XML tag.
     *
     * \return The frame rate to be used for rendering
     *
     * \see projectInformation
     * \see m_UserData
     */
    int graphicsFrameRate() const;

    /**
     * \brief Retrieves the quality setting at which the project's graphics application should be rendered
     *
     * This setting is given by the \c quality attribute of the \c graphics tag that's
     * part of the \c project_specific XML tag.
     *
     * \return The quality setting to be used for rendering
     *
     * \see GraphicsQualitySetting
     * \see projectInformation
     * \see m_UserData
     */
    GraphicsQualitySetting graphicsQualitySetting() const;

    /**
     * \brief Retrieves the initial window width when running in windowed mode
     *
     * This setting is given by the \c width attribute of the \c graphics tag that's
     * part of the \c project_specific XML tag. It's ignored when the application
     * is started in fullscreen/screensaver mode.
     *
     * \return The initial window width to be used
     *
     * \see projectInformation
     * \see m_UserData
     */
    int graphicsWindowWidth() const;

    /**
     * \brief Retrieves the initial window height when running in windowed mode
     *
     * This setting is given by the \c height attribute of the \c graphics tag that's
     * part of the \c project_specific XML tag. It's ignored when the application
     * is started in fullscreen/screensaver mode.
     *
     * \return The initial window height to be used
     *
     * \see projectInformation
     * \see m_UserData
     */
    int graphicsWindowHeight() const;

private:
    /**
     * \brief Fetch the contents of \c init_data.xml
     *
     * This method uses the BOINC API in order to fill the \c APP_INIT_DATA structure m_UserData
     * with initial information about the current work unit computation session (slot). The data
     * in \c init_data.xml is refreshed only at the beginning of a session, hence this method doesn't
     * need to be called periodically.
     */
	void readUserInfo();

	/**
	 * \brief Fetch the contents of the shared memory area provided by the \b Einstein\@Home application
	 *
	 * The shared memory area contains various informational bits and pieces about the running application
	 * and work unit computation. The contents have to be considered as volatile, hence should be refreshed
	 * periodically.
	 *
	 * \see refresh()
	 */
	void readSharedMemoryArea();

	/**
	 * \brief Parse the project-specific preferences (XML)
	 *
	 * The project-specific preferences are retrieved by projectInformation().
	 * This method parses the XML string returned and populates the respective data members.
	 *
	 * \see projectInformation()
	 * \see m_GraphicsFrameRate
	 * \see m_GraphicsQualitySetting
	 * \see m_GraphicsWindowWidth
	 * \see m_GraphicsWindowHeight
	 */
	void readProjectPreferences();

	/// State flag which indicates whether the adapter instance is ready to be used
	bool m_Initialized;

	/// Name tag used to identify the shared memory area provided by the \b Einstein\@Home application
	string m_SharedMemoryAreaIdentifier;

	/// Pointer to the shared memory area
	char *m_SharedMemoryArea;

	/// The contents of the shared memory area after the last refresh
	string m_SharedMemoryAreaContents;

	/// Flag to indicate whether the shared memory area is available or not
	bool m_SharedMemoryAreaAvailable;

	/// Frame rate at which the project's graphics application should be rendered
	int m_GraphicsFrameRate;

	/// Quality setting at which the project's graphics application should be rendered
	GraphicsQualitySetting m_GraphicsQualitySetting;

	/// Initial window width when running in windowed mode
	int m_GraphicsWindowWidth;

	/// Initial window height when running in windowed mode
	int m_GraphicsWindowHeight;

	/**
	 * \brief Information structure returned by the BOINC client API.
	 *
	 * It contains information about the currently active project, science application,
	 * user account, work unit and computation session.
	 */
	APP_INIT_DATA m_UserData;

	/// Pointer to the XML processor
	XMLProcessorInterface* m_xmlIFace;
};

/**
 * @}
 */

#endif /*BOINCCLIENTADAPTER_H_*/
