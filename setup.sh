#!/bin/sh

###########################################################################
#   Copyright (C) 2008 by Oliver Bock                                     #
#   oliver.bock[AT]aei.mpg.de                                             #
#                                                                         #
#   This file is part of Einstein@Home.                                   #
#                                                                         #
#   Einstein@Home is free software: you can redistribute it and/or modify #
#   it under the terms of the GNU General Public License as published     #
#   by the Free Software Foundation, version 2 of the License.            #
#                                                                         #
#   Einstein@Home is distributed in the hope that it will be useful,      #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          #
#   GNU General Public License for more details.                          #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with Einstein@Home. If not, see <http://www.gnu.org/licenses/>. #
#                                                                         #
###########################################################################

### TODOs #################################################################

# error level checking (break in case of error, always 2>&1 for make logging)
# more refactoring (every lib in dedicated function? per traget?)

### functions #############################################################

check_prerequisites()
{
	echo "Not yet implemented: check_prerequisites()"
	# automake
	# autoconf
	# cmake
	# wget
	# C compiler
	# lex or flex
	# yacc or bison or byacc
	# m4
}


prepare_generic()
{
	cd $ROOT

	echo "Preparing source tree..." | tee -a $ROOT/setup.log
	mkdir -p 3rdparty/sdl >> $ROOT/setup.log
	mkdir -p 3rdparty/freetype2 >> $ROOT/setup.log
	mkdir -p 3rdparty/oglft >> $ROOT/setup.log
	mkdir -p 3rdparty/boinc >> $ROOT/setup.log
	
	echo "Preparing build tree..." | tee -a $ROOT/setup.log
	mkdir -p build/sdl >> $ROOT/setup.log
	mkdir -p build/freetype2 >> $ROOT/setup.log
	mkdir -p build/oglft >> $ROOT/setup.log
	mkdir -p build/boinc >> $ROOT/setup.log
	mkdir -p build/framework >> $ROOT/setup.log
	mkdir -p build/orc >> $ROOT/setup.log
	mkdir -p build/starsphere >> $ROOT/setup.log
	
	echo "Preparing install tree..." | tee -a $ROOT/setup.log
	mkdir -p install/bin >> $ROOT/setup.log
	mkdir -p install/include >> $ROOT/setup.log
	mkdir -p install/lib >> $ROOT/setup.log
	
	# prepare additional sources
	
	cd $ROOT/3rdparty/sdl
	if [ -d .svn ]; then
		echo "Updating SDL..." | tee -a $ROOT/setup.log
		svn update >> $ROOT/setup.log
	else
		echo "Retrieving SDL..." | tee -a $ROOT/setup.log
		svn checkout http://svn.libsdl.org/branches/SDL-1.2 . >> $ROOT/setup.log
	fi
	
	cd $ROOT/3rdparty/freetype2
	if [ -d CVS ]; then
		echo "Updating Freetype2..." | tee -a $ROOT/setup.log
		cvs update -C >> $ROOT/setup.log 2>&1
	else
		cd ..
		echo "Retrieving Freetype2..." | tee -a $ROOT/setup.log
		cvs -z3 -d:pserver:anonymous@cvs.sv.gnu.org:/sources/freetype checkout -r VER-2-3-5-REAL freetype2 >> $ROOT/setup.log 2>&1
	fi
	
	cd $ROOT/3rdparty/oglft
	if [ -d .svn ]; then
		echo "Updating OGLFT..." | tee -a $ROOT/setup.log
		svn update >> ../../setup.log
	else
		echo "Retrieving OGLFT..." | tee -a $ROOT/setup.log
		svn checkout https://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> $ROOT/setup.log
	fi
	
	cd $ROOT/3rdparty/boinc
	if [ -d .svn ]; then
		echo "Updating BOINC..." | tee -a $ROOT/setup.log
		svn update >> $ROOT/setup.log
	else
		echo "Retrieving BOINC..." | tee -a $ROOT/setup.log
		svn checkout http://boinc.berkeley.edu/svn/trunk/boinc . >> $ROOT/setup.log
	fi
}


prepare_win32()
{
	cd $ROOT

	echo "Preparing MinGW source tree..." | tee -a $ROOT/setup.log
	mkdir -p 3rdparty/mingw/xscripts >> $ROOT/setup.log
	cd 3rdparty/mingw/xscripts

	if [ -d CVS ]; then
		echo "Updating MinGW build script..." | tee -a $ROOT/setup.log
		cvs update -C >> $ROOT/setup.log 2>&1
	else
		cd ..
		echo "Retrieving MinGW build script..." | tee -a $ROOT/setup.log
		cvs -z3 -d:pserver:anonymous@mingw.cvs.sourceforge.net:/cvsroot/mingw checkout -P xscripts >> $ROOT/setup.log 2>&1
	fi
	
	cd $ROOT/3rdparty/mingw/xscripts
	echo "Preparing MinGW build script..." | tee -a $ROOT/setup.log
	# note: svn has no force/overwrite switch. the file might not be updated when patched
	patch x86-mingw32-build.sh.conf < $ROOT/patches/x86-mingw32-build.sh.conf.patch >> $ROOT/setup.log
	chmod +x x86-mingw32-build.sh >> $ROOT/setup.log
}


build_generic()
{
	cd $ROOT/3rdparty/sdl
	echo "Building SDL (this may take a while)..." | tee -a $ROOT/setup.log
	./autogen.sh >> $ROOT/setup.log
	cd $ROOT/build/sdl
	$ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $ROOT/setup.log
	make >> $ROOT/setup.log
	make install >> $ROOT/setup.log
	echo "Successfully built and installed SDL!" | tee -a $ROOT/setup.log

	cd $ROOT/3rdparty/freetype2
	echo "Building Freetype2 (this may take a while)..." | tee -a $ROOT/setup.log
	chmod +x autogen.sh >> $ROOT/setup.log
	chmod +x configure >> $ROOT/setup.log
	#./autogen.sh >> $ROOT/setup.log
	cd $ROOT/build/freetype2
	# note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
	$ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $ROOT/setup.log
	make >> $ROOT/setup.log
	make install >> $ROOT/setup.log
	echo "Successfully built and installed Freetype2!" | tee -a $ROOT/setup.log

	cd $ROOT/3rdparty/oglft/liboglft
	echo "Building OGLFT (this may take a while)..." | tee -a $ROOT/setup.log
	# note: svn has no force/overwrite switch. the file might not be updated when patched
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.liboglft.patch >> $ROOT/setup.log
	cd $ROOT/build/oglft
	cmake $ROOT/3rdparty/oglft >> $ROOT/setup.log
	make >> $ROOT/setup.log
	mkdir -p $ROOT/install/include/oglft >> $ROOT/setup.log
	cp OGLFT.h $ROOT/install/include/oglft >> $ROOT/setup.log
	cp liboglft/liboglft.a $ROOT/install/lib >> $ROOT/setup.log
	echo "Successfully built and installed OGLFT!" | tee -a $ROOT/setup.log

	cd $ROOT/3rdparty/boinc
	echo "Building BOINC (this may take a while)..." | tee -a $ROOT/setup.log
	./_autosetup >> $ROOT/setup.log
	cd $ROOT/build/boinc
	$ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client >> $ROOT/setup.log
	make >> $ROOT/setup.log
	make install >> $ROOT/setup.log
	echo "Successfully built and installed BOINC!" | tee -a $ROOT/setup.log
}

build_mingw()
{
	echo "Not yet implemented: build_mingw()"
}

build_starsphere()
{
	echo "Building Starsphere [ORC] (this may take a while)..." | tee -a $ROOT/setup.log
	export ORC_SRC=$ROOT/src/orc
	export ORC_INSTALL=$ROOT/install
	cd $ROOT/build/orc
	cp $ROOT/src/orc/Makefile .  >> $ROOT/setup.log
	make >> $ROOT/setup.log
	make install >> $ROOT/setup.log
	echo "Successfully built and installed Starsphere [ORC]!" | tee -a $ROOT/setup.log

	echo "Building Starsphere [Framework] (this may take a while)..." | tee -a $ROOT/setup.log
	export FRAMEWORK_SRC=$ROOT/src/framework
	export FRAMEWORK_INSTALL=$ROOT/install
	cd $ROOT/build/framework
	cp $ROOT/src/framework/Makefile .  >> $ROOT/setup.log
	make >> $ROOT/setup.log
	make install >> $ROOT/setup.log
	echo "Successfully built and installed Starsphere [Framework]!" | tee -a $ROOT/setup.log

	echo "Building Starsphere [Application] (this may take a while)..." | tee -a $ROOT/setup.log
	export STARSPHERE_SRC=$ROOT/src/starsphere
	export STARSPHERE_INSTALL=$ROOT/install
	cd $ROOT/build/starsphere
	cp $ROOT/src/starsphere/Makefile .  >> $ROOT/setup.log
	cp $ROOT/src/starsphere/*.res .  >> $ROOT/setup.log
	make >> $ROOT/setup.log
	make install >> $ROOT/setup.log
	echo "Successfully built and installed Starsphere [Application]!" | tee -a $ROOT/setup.log
}

build_linux()
{
	build_generic
	build_starsphere
}


build_mac()
{
	build_generic
	build_starsphere
}


build_win32()
{
	build_mingw
	build_generic
	build_starsphere
}


check_last_build()
{
	LASTBUILD=`cat .lastbuild`

	if [ "$LASTBUILD" != "$1" ]; then
		# TODO: clean build dirs when different target!
		echo "Not yet implemented: cleanup in check_lastbuild()"
	fi

	echo "$1" > .lastbuild
}

print_usage()
{
	cd $ROOT

	echo "*************************"
	echo "Usage: `basename $0` <target>"
	echo
	echo "Available targets:"
	echo "  --linux"
	echo "  --mac"
	echo "  --win32"
	echo "*************************"

	echo "Wrong usage. Stopping!" >> $ROOT/setup.log
}


### main control ##########################################################

ROOT=`pwd`
DATE=`date`

TARGET=0
TARGET_LINUX=1
TARGET_MAC=2
TARGET_WIN32=3

echo "************************************" >> $ROOT/setup.log
echo "Starting new setup run!" >> $ROOT/setup.log
echo "$DATE" >> $ROOT/setup.log
echo "************************************" >> $ROOT/setup.log

# crude command line parsing :-)

if [ $# -ne 1 ]; then
  print_usage
  exit 1
fi

case "$1" in
	"--linux")
		TARGET=$TARGET_LINUX
		check_last_build "$1"
		echo "Building linux version:" | tee -a $ROOT/setup.log
		;;
	"--mac")
		TARGET=$TARGET_MAC
		check_last_build "$1"
		echo "Building mac version:" | tee -a $ROOT/setup.log
		;;
	"--win32")
			TARGET=$TARGET_WIN32
			check_last_build "$1"
			echo "Building win32 version:" | tee -a $ROOT/setup.log
			;;
	*)
			print_usage
			exit 1
			;;
esac

# here we go...

check_prerequisites
prepare_generic

if [ TARGET=$TARGET_LINUX ]; then
	build_linux
fi

exit 0
