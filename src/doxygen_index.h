/*! \mainpage Overview
 *
 * \section cha_intro Introduction
 *
 * This application is part of the Einstein\@Home project and aims at providing an extensible refurbished version of
 * the well-known %Starsphere graphics application (screensaver).
 * \n
 * The new version currently available version focusses on the following topics:
 * 
 * - Refurbished visuals and extended features
 * - Generalized open source framework (API) for community-driven implementations 
 * - Migration from GLUT to SDL 
 * - Self-consistent build script (Linux, Mac OS X, Windows/MinGW)
 * - Full doxygen documentation
 *
 * \section sec_doc_user User Documentation
 * 
 * \subsection cha_app The Starsphere Graphics Application
 * 
 * \subsubsection sec_app_overview Overview
 * 
 * The %Starsphere application is based on the original version currently shipped with Einstein\@Home.
 * For more infomation about what it shows please refer to our <a href="http://einstein.phys.uwm.edu/starsphere.php">project page</a>.
 * \n
 * With regard to features the following extensions have been made so far:
 * - The VIRGO interferometer is shown (in white) at 43°37'53"N and 10°30'18"E
 * - The celestial globe is displayed indicating latitude, longitude and the projected Greenwich Meridian
 * - The different screen elements can be toggled separately
 * 
 * \subsubsection sec_app_usage Usage
 * 
 * Use the following keys to control the application:
 * - <code>ENTER</code>: toggle fullscreen mode
 * - <code>ESC</code>: exit the application
 * 
 * Use your mouse to modify the 3D rendering by click & drag:
 * - Left button: rotate the star sphere
 * - Right button: zoom in or out
 * 
 * Use the following keys to toggle single screen elements:
 * - <code>S</code>: Stars
 * - <code>C</code>: Constellations
 * - <code>P</code>: Pulsars
 * - <code>R</code>: Supernova Remnants
 * - <code>X</code>: X-Ray Binary Systems
 * - <code>O</code>: Observatories
 * - <code>M</code>: Search Marker
 * - <code>I</code>: Search Information
 * - <code>G</code>: Globe
 * - <code>L</code>: Logo
 * 
 * Available command line options:
 * - Initial fullscreen mode: \code --fullscreen \endcode
 *  
 * \subsubsection sec_app_install Installation
 *
 * \section sec_doc_dev Developer Documentation
 * 
 * \subsection cha_frame The Framework
 * \subsubsection sec_frame_overview Overview
 *  
 * The Framework takes care of the following things:
 * 
 * - Window management
 * - 3D context management
 * - Event handling (mouse, keyboard)
 * - Interface to BOINC application
 * - Interface to Einstein\@Home science application
 * 
 * For a detailed description of the available classes and interfaces please refer to the <a href="annotated.html">class list</a>
 *
 * \subsection cha_orc The Open Resource Compiler
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
 * -# Run the resource compiler (<code>orc</code>) to convert the resources into source code
 * -# Include the generated source file into your project (compile and link it to main executable)
 * -# Use ResourceFactory to instantiate a Resource object (loads the binary resource data into memory)
 * -# Use the Resource instance to retrieve a pointer to the resource's raw data
 * 
 * %Resource specification file format:
 * - Simple text file (*.orc)
 * - Each line describes one resource
 * - The descriptor has to look like this: <code>LogicalResourceName|PhysicalResourceName</code>
 * - <code>PhysicalResourceName</code> is the actual file name of the resource
 * - Lines starting with # are treated as comments
 * - Empty lines are ignored
 * 
 * Command line calling convention:
 * \code orc <InputSpecFileName> <OutputCodeFileName> \endcode
 * - <code>InputSpecFileName</code>: The name of the resource specification file (input)
 * - <code>OutputCodeFileName</code>: The name of the resource code file (output)
 * 
 * \subsection cha_source The Source Code
 * 
 * You may check out (download) the source code from our CVS repository:
 * \code
 * cvs -d:pserver:anonymous@gravity.phys.uwm.edu:2402/usr/local/cvs/lscsoft login
 * cvs -z3 -d:pserver:anonymous@gravity.phys.uwm.edu:2402/usr/local/cvs/lscsoft co -P einsteinathome/graphics
 * \endcode
 *
 * \subsection cha_build The Build Script
 * 
 * \subsubsection sec_orc_overview Overview
 * 
 * The build script tries to compile binaries for all supported platforms without denpending
 * on any required library being installed on the build machine. The only requirement is a properly
 * installed and configured build system based on the GNU Compiler Collection Version 4 and a
 * working (direct) connection to the internet. Everything else is subsequently downloaded,
 * configured and compiled automatically by the script.
 * 
 * \subsubsection sec_orc_usage Requirements
 * 
 * - bash
 * - gcc / g++
 * - automake
 * - autoconf
 * - m4
 * - cmake
 * - wget
 * - ld
 * - libtool
 * - ar
 * - lex (i.e. flex)
 * - yacc (i.e. bison)
 * 
 * \subsubsection sec_orc_usage Usage
 * 
 * In order to use the script you just have to navigate to the <code>graphics</code> directory
 * in the Einstein\@Home source tree and run the script passing as an argument the desired target platform: 
 * 
 * - Linux build: \code build.sh --linux \endcode
 * - Mac OS X build: \code build.sh --mac \endcode
 * - Windows build (uses MinGW cross-compile with Linux as build system!): \code build.sh --win32 \endcode
 * - Purge build tree: \code build.sh --distclean \endcode
 * 
 * \section cha_future The Future
 * 
 * - Community implementations!
 * - Different backgound and search visualizations
 * - Full linux screensaver support (X11, KDE, Gnome)
 * - Options should be saved persistently
 * - Quality settings (reduce CPU consumption)
 * - Help dialog (on screen display)
 * 
 * \section cha_license The Licence
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
