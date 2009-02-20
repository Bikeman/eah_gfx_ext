/**
 * \mainpage Overview
 *
 * \section cha_intro Introduction
 *
 * This application is part of the Einstein\@Home project and aims to provide an extensible, refurbished version of
 * the well-known %Starsphere graphics application (screensaver).
 * \n
 * The new version focuses on the following topics:
 *
 * - Refurbished visuals and extended features
 * - Generalized API for community-driven implementations
 * - %Resource embedding (e.g. fonts, images)
 * - Migration from GLUT to SDL
 * - Self-consistent build script (Linux, Mac OS X, Windows/MinGW)
 * - Full doxygen documentation
 *
 * \subsection sec_status Current Status
 *
 * The graphics application is currently in its beta stage and has still to be considered as work-in-progress.
 * This means that probably not all desired features are fully implemented yet and the code might still contain some bugs.
 * However, we think the application is stable enough to be used on a daily basis. You're invited to test and
 * discuss the current version at the Einstein\@Home
 * \htmlonly<a href="http://einstein.phys.uwm.edu/forum_forum.php?id=3">\endhtmlonly
 * Cruncher's Corner
 * \htmlonly</a>\endhtmlonly.
 *
 * The most important feature that's not fully supported right now is the persistent configuration of the graphics
 * application. It is intended to let users customize the application's options using the BOINC project preferences
 * and/or other means. This task is still pending but is scheduled for the near future. As soon as these changes are
 * completed users will be able to set the following options:
 *
 * - Common options (already available):
 *   - Window width (default: 800 pixels)
 *   - Window height (default: 600 pixels)
 *   - Frame rate (default: 20 fps)
 *   - Quality (default: low / also available: medium, high)
 * - %Starsphere options (still pending):
 *   - Enabled screen elements (see \ref sec_app_usage)
 *
 * Please note that we're using conservative default values (for the common options) to ensure that the graphics
 * applications runs with reasonable CPU consumption on most systems. This means that the visual improvements are
 * \b not activated and the graphics look more or less the same as before. If you have hardware accelerated 3D
 * support on your machine please feel free to increase the settings. For example: on a recent Linux PC the
 * %Starsphere application consumes less than 10% CPU power when running at typical desktop resolutions (e.g. 1280x1024),
 * 30 frames per second and high quality.
 *
 * \subsection sec_platforms Supported Platforms
 *
 * The binary versions as well as the build script support the following platforms:
 * - Linux (Intel, 32 Bit)
 * - Mac OS X (Tiger/Leopard, Intel/PowerPC, 32 Bit)
 * - Windows (Intel, 32 Bit, cross compiled with MinGW)
 *
 * \subsection sec_future The Future
 *
 * - Community involvement: build and share \b your \b own graphics application!
 * - Provide different backgrounds and search visualizations
 * - Full Linux screensaver support (X11, KDE, Gnome)
 * - Help dialog (on screen display)
 *
 * \section sec_doc_user User Documentation
 *
 * \subsection cha_app_S5 Starsphere Graphics Application - LSC/Virgo S5
 *
 * \subsubsection sec_app_overview Overview
 *
 * The new %Starsphere application is based on the original version currently shipped with Einstein\@Home.
 * For more information about what it shows please refer to our
 * \htmlonly<a href="http://einstein.phys.uwm.edu/starsphere.php">\endhtmlonly
 * project page
 * \htmlonly</a>\endhtmlonly.
 * \n
 * With regard to features the following extensions have been made so far:
 * - The VIRGO interferometer is shown (in white) at 43°37'53"N and 10°30'18"E
 * - The different screen elements can be toggled separately
 *
 * \subsubsection sec_app_usage Usage
 *
 * When the graphics application is started in standalone mode (as opposed to screensaver mode)
 * you can control the graphics interactively using mouse and keyboard.
 *
 * Use the following keys to control the application:
 * - \c ENTER: Toggle fullscreen mode
 * - \c ESC: Exit the application
 *
 * Use your mouse to modify the 3D rendering (click & drag):
 * - Left button: Rotate the sphere
 * - Right button: Zoom in or out
 *
 * Use the following keys to toggle single screen elements:
 * - \c S: Stars
 * - \c C: Constellations
 * - \c P: Pulsars
 * - \c R: Supernova Remnants
 * - \c X: X-Ray Binary Systems
 * - \c O: Observatories
 * - \c M: Search Marker
 * - \c I: Search Information
 * - \c G: Globe
 * - \c L: Logo
 *
 * Available command line options:
 * - Screensaver mode (mouse and keyboard control deactivated!): \code --fullscreen \endcode
 *
 * \subsubsection sec_app_install Installation
 *
 * You don't have to install the graphics application yourself as it's distributed alongside
 * Einstein\@Home.
 *
 * \subsection cha_app_Radio Starsphere Graphics Application - Arecibo Binary Pulsar Search
 *
 * In addition to the well known Einstein\@Home application there's a second application that
 * searches for tight binary pulsar systems in data recorded at the Arecibo telescope in Puerto
 * Rico, USA. This search comes with a slightly different graphics application that displays specific
 * search parameters and other real-time visualizations special to this data analysis method. The
 * main differences are:
 *
 * - A real-time power-spectrum of the data being searched (candidates are shown as bright white lines!)
 * - Additional search parameters showing orbital properties of the binary pulsar system being searched
 * - The real-time location of the Arecibo telescope
 * - Project subtitle: "International Year of Astronomy 2009"
 *
 * Details about this particular search application and how it's related to the search for
 * gravitational waves will be published on the Einstein@Home website at the official launch.
 *
 * \section sec_doc_dev Developer Documentation
 *
 * \subsection cha_frame Graphics Application Framework
 * \subsubsection sec_frame_overview Overview
 *
 * The Framework takes care of the following things:
 *
 * - Window management
 * - OpenGL 3D context management
 * - Event handling (mouse, keyboard)
 * - Interface to BOINC application
 * - Interface to Einstein\@Home science application
 *
 * For a detailed description of the available classes and interfaces please refer to the
 * \htmlonly<a href="annotated.html">\endhtmlonly
 * class list
 * \htmlonly</a>\endhtmlonly.
 *
 * \subsection cha_orc Open Resource Compiler
 *
 * \subsubsection sec_orc_overview Overview
 *
 * The resource compiler is a very simple tool that converts binary resource files like images or fonts into plain C++ source
 * code which can subsequently be compiled and linked to the executable. This way you can integrate any resource into your
 * binary which no longer depends on external files, hence facilitates later distribution. For a detailed description please
 * have a look at the ResourceCompiler class.
 *
 * \subsubsection sec_orc_usage Usage
 *
 * -# Create a resource specification file
 * -# Run the resource compiler (\c orc) to convert the resources into source code
 * -# Include the generated source file in your project (compile and link it to the main executable)
 * -# Use ResourceFactory to instantiate a Resource object (loads the binary resource data into memory)
 * -# Use the Resource instance to retrieve a pointer to the resource's raw data
 *
 * %Resource specification file format:
 * - Simple text file (*.orc)
 * - Each line describes one resource
 * - The descriptor has to look like this: \c LogicalResourceName|PhysicalResourceName
 * - \c PhysicalResourceName is the actual file name of the resource
 * - Lines starting with # are treated as comments
 * - Empty lines are ignored
 *
 * Command line calling convention:
 * \code orc <InputSpecFileName> <OutputCodeFileName> \endcode
 * - \c InputSpecFileName: The name of the resource specification file (input)
 * - \c OutputCodeFileName: The name of the resource code file (output)
 *
 * \subsection cha_source Source Code Access
 *
 * Downloads of the source code are available as tarball or zip-archive. Please visit the
 * \htmlonly<a href="https://git.aei.uni-hannover.de/cgi-bin/gitweb.cgi?p=shared/einsteinathome/graphicsframework.git;a=summary" target="_blank">\endhtmlonly
 * official repository
 * \htmlonly</a>\endhtmlonly
 * and click on the "snapshot" download link
 * next to the desired commit/version (see section "shortlog").
 *
 * You may also check out (clone) the source code from the
 * \htmlonly<a href="http://git-scm.com" target="_blank">\endhtmlonly
 * git
 * \htmlonly</a>\endhtmlonly
 * repository like this:
 * \code
 * git clone git://git.aei.uni-hannover.de/shared/einsteinathome/graphicsframework.git
 * \endcode
 *
 * \subsection cha_build Build Script
 *
 * \subsubsection sec_build_overview Overview
 *
 * The (very simple) build script tries to compile binaries for all supported platforms without depending
 * on any extraordinary library being installed on the build machine. The only requirements are a properly
 * installed and configured build system based on the GNU Compiler Collection Version 4, a few common
 * library development packages and a working (direct) connection to the internet.
 * Everything else is subsequently downloaded, configured and compiled automatically by the script.
 *
 * \subsubsection sec_build_provided Libraries used (provided automatically)
 *
 * - SDL - Simple DirectMedia Layer
 * - FreeType 2 - High-quality glyph images
 * - libxml2 - Full XML support (DOM, SAX, XSLT, Validation)
 * - OGLFT - OpenGL-FreeType Library
 * - BOINC (Graphics/API)
 *
 * \subsubsection sec_build_requirements Requirements
 *
 * Please make sure the following development packages (headers/libs) and tools are available to the build script
 * (note: the following package names refer to Debian only, your distribution's package names might vary):
 *
 * - Common libraries (mostly required by SDL):
 *   - libc6-dev
 *   - zlib1g-dev
 * - Libraries for Linux and Windows/MinGW (required by SDL):
 *   - libgl1-mesa-dev | nvidia-glx-dev
 *   - libglu1-mesa-dev
 *   - libdirectfb-dev
 *   - libxt-dev
 *   - libxext-dev
 *   - libaudio-dev
 * - Libraries for Mac OS X (required by SDL, provided by XCode):
 *   - OpenGL Framework
 *   - Cocoa Framework
 *   - Carbon Framework
 *   - QuickTime Framework
 *   - ApplicationServices Framework
 *   - AudioToolbox Framework
 *   - AudioUnit Framework
 *   - IOKit Framework
 * - Tools:
 *   - bash
 *   - gcc / g++
 *   - automake
 *   - autoconf
 *   - m4
 *   - cmake
 *   - pkg-config
 *   - wget
 *   - cvs
 *   - svn
 *   - tar
 *   - patch
 *   - ld
 *   - libtool
 *   - ar
 *   - lex (i.e. flex)
 *   - yacc (i.e. bison)
 *   - doxygen (optional)
 *
 * \subsubsection sec_build_usage Usage
 *
 * In order to use the script you just have to navigate to the \c graphics directory
 * in the Einstein\@Home source tree and run the script passing as an argument the desired target platform
 * (you'll find the compiled executables \c starsphere_EinsteinS5R3 and \c starsphere_Radio in the
 * \c install/bin directory):
 *
 * - Linux build: \code build.sh --linux \endcode
 * - Mac OS X build: \code build.sh --mac-intel \endcode \code build.sh --mac-ppc \endcode
 * - Windows build (uses MinGW cross-compile with Linux as build system!): \code build.sh --win32 \endcode
 *
 * Additional tools are provided as follows:
 *
 * - Build this documentation (requires doxygen): \code build.sh --doc \endcode
 * - Purge build tree: \code build.sh --distclean \endcode
 *
 * \section cha_license License
 * \verbatim
   Copyright © 2004 Bruce Allen, David Hammer, Eric Myers
   Copyright © 2008 Oliver Bock, Bernd Machenschalk

   This application/framework is part of Einstein@Home.

   Einstein@Home is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation, version 2 of the License.

   Einstein@Home is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>.
   \endverbatim
 */
