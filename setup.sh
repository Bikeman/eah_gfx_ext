#!/bin/bash

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

### globals ###############################################################

ROOT=`pwd`
DATE=`date`
LOGFILE=$ROOT/setup.log
TARGET=0

### functions #############################################################

failure()
{
	echo "************************************" | tee -a 
	echo "Error detected! Stopping setup run!" | tee -a $LOGFILE
	echo "$DATE" | tee -a $LOGFILE

    if [ -f "$LOGFILE" ]; then
		echo "------------------------------------"
		echo "Please check logfile: `basename $LOGFILE`"
		echo "These are the final ten lines:"
		echo "------------------------------------"
		tail -10 $LOGFILE
    fi

	echo "************************************" | tee -a $LOGFILE

    exit 1
}

check_prerequisites()
{
	echo "Checking prerequisites..."
	
	# required toolchain
	TOOLS="automake autoconf m4 cmake wget gcc g++ ld libtool ar lex yacc"

	for tool in $TOOLS; do
		if ! ( type $tool >/dev/null 2>&1 ); then
			echo "Error! Missing \"$tool\" which is a required tool! Stopping..."
			return 1
		fi
	done

	return 0
}


prepare_generic()
{
	cd $ROOT

	echo "Preparing source tree..." | tee -a $LOGFILE
	mkdir -p 3rdparty/oglft >> $LOGFILE
	mkdir -p 3rdparty/boinc >> $LOGFILE
	
	echo "Preparing build tree..." | tee -a $LOGFILE
	mkdir -p build/sdl >> $LOGFILE
	mkdir -p build/freetype2 >> $LOGFILE
	mkdir -p build/oglft >> $LOGFILE
	mkdir -p build/boinc >> $LOGFILE
	mkdir -p build/framework >> $LOGFILE
	mkdir -p build/orc >> $LOGFILE
	mkdir -p build/starsphere >> $LOGFILE
	
	echo "Preparing install tree..." | tee -a $LOGFILE
	mkdir -p install/bin >> $LOGFILE
	mkdir -p install/include >> $LOGFILE
	mkdir -p install/lib >> $LOGFILE
	
	# prepare additional sources
	
	mkdir -p 3rdparty/sdl >> $LOGFILE
	cd $ROOT/3rdparty/sdl
	if [ -d .svn ]; then
		echo "Updating SDL..." | tee -a $LOGFILE
		svn update >> $LOGFILE
	else
		echo "Retrieving SDL (this may take a while)..." | tee -a $LOGFILE
		svn checkout http://svn.libsdl.org/branches/SDL-1.2 . >> $LOGFILE
	fi

# 	cd $ROOT/3rdparty
# 	echo "Retrieving SDL (this may take a while)..." | tee -a $LOGFILE
# 	wget http://www.libsdl.org/release/SDL-1.2.14.tar.gz >> $LOGFILE 2>&1
# 	tar -xzf SDL-1.2.14.tar.gz >> $LOGFILE
# 	rm SDL-1.2.14.tar.gz >> $LOGFILE
# 	mv SDL-1.2.14 sdl >> $LOGFILE
	
	cd $ROOT/3rdparty
	echo "Retrieving Freetype2 (this may take a while)..." | tee -a $LOGFILE
	wget http://mesh.dl.sourceforge.net/sourceforge/freetype/freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1
	tar -xjf freetype-2.3.5.tar.bz2 >> $LOGFILE
	rm freetype-2.3.5.tar.bz2 >> $LOGFILE
	mv freetype-2.3.5 freetype2 >> $LOGFILE
	
	cd $ROOT/3rdparty/oglft
	if [ -d .svn ]; then
		echo "Updating OGLFT..." | tee -a $LOGFILE
		svn update >> ../../setup.log
	else
		echo "Retrieving OGLFT (this may take a while)..." | tee -a $LOGFILE
		svn checkout https://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> $LOGFILE
	fi
	
	cd $ROOT/3rdparty/boinc
	if [ -d .svn ]; then
		echo "Updating BOINC..." | tee -a $LOGFILE
		svn update >> $LOGFILE
	else
		echo "Retrieving BOINC (this may take a while)..." | tee -a $LOGFILE
		svn checkout http://boinc.berkeley.edu/svn/trunk/boinc . >> $LOGFILE
	fi

	return 0
}


prepare_win32()
{
	cd $ROOT

	echo "Preparing MinGW source tree..." | tee -a $LOGFILE
	mkdir -p 3rdparty/mingw/xscripts >> $LOGFILE
	cd 3rdparty/mingw/xscripts

	if [ -d CVS ]; then
		echo "Updating MinGW build script..." | tee -a $LOGFILE
		cvs update -C >> $LOGFILE 2>&1
	else
		cd ..
		echo "Retrieving MinGW build script (this may take a while)..." | tee -a $LOGFILE
		cvs -z3 -d:pserver:anonymous@mingw.cvs.sourceforge.net:/cvsroot/mingw checkout -P xscripts >> $LOGFILE 2>&1
	fi
	
	cd $ROOT/3rdparty/mingw/xscripts
	echo "Preparing MinGW build script..." | tee -a $LOGFILE
	# note: svn has no force/overwrite switch. the file might not be updated when patched
	patch x86-mingw32-build.sh.conf < $ROOT/patches/x86-mingw32-build.sh.conf.patch >> $LOGFILE
	chmod +x x86-mingw32-build.sh >> $LOGFILE

	return 0
}


build_generic()
{
	cd $ROOT/3rdparty/sdl
	echo "Building SDL (this may take a while)..." | tee -a $LOGFILE
	./autogen.sh >> $LOGFILE
	cd $ROOT/build/sdl
	$ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE
	make >> $LOGFILE
	make install >> $LOGFILE
	echo "Successfully built and installed SDL!" | tee -a $LOGFILE

	cd $ROOT/3rdparty/freetype2
	echo "Building Freetype2 (this may take a while)..." | tee -a $LOGFILE
	chmod +x autogen.sh >> $LOGFILE
	chmod +x configure >> $LOGFILE
	./autogen.sh >> $LOGFILE
	cd $ROOT/build/freetype2
	# note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
	$ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE
	make >> $LOGFILE
	make install >> $LOGFILE
	echo "Successfully built and installed Freetype2!" | tee -a $LOGFILE

	cd $ROOT/3rdparty/oglft
	echo "Patching OGLFT..." | tee -a $LOGFILE
	# note: svn has no force/overwrite switch. patched files might not be updated
	# patch: use fixed settings for freetype, deactivate FindFreetype
	FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
	FREETYPE2_LIBRARIES="$ROOT/install/lib/liboglft.a"
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE
	# patch: build static lib instead of shared
	cd $ROOT/3rdparty/oglft/liboglft
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.liboglft.patch >> $LOGFILE
	echo "Building OGLFT..." | tee -a $LOGFILE
	cd $ROOT/build/oglft
	cmake -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE
	make >> $LOGFILE
	mkdir -p $ROOT/install/include/oglft >> $LOGFILE
	cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE
	cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE
	echo "Successfully built and installed OGLFT!" | tee -a $LOGFILE

	cd $ROOT/3rdparty/boinc
	echo "Building BOINC (this may take a while)..." | tee -a $LOGFILE
	./_autosetup >> $LOGFILE
	cd $ROOT/build/boinc
	$ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client >> $LOGFILE
	make >> $LOGFILE
	make install >> $LOGFILE
	echo "Successfully built and installed BOINC!" | tee -a $LOGFILE

	return 0
}


build_mingw()
{
	echo "Not yet implemented: build_mingw()"

	return 0
}


build_starsphere()
{
	echo "Building Starsphere [ORC]..." | tee -a $LOGFILE
	export ORC_SRC=$ROOT/src/orc
	export ORC_INSTALL=$ROOT/install
	cd $ROOT/build/orc
	cp $ROOT/src/orc/Makefile .  >> $LOGFILE
	make >> $LOGFILE
	make install >> $LOGFILE
	echo "Successfully built and installed Starsphere [ORC]!" | tee -a $LOGFILE

	echo "Building Starsphere [Framework]..." | tee -a $LOGFILE
	export FRAMEWORK_SRC=$ROOT/src/framework
	export FRAMEWORK_INSTALL=$ROOT/install
	cd $ROOT/build/framework
	cp $ROOT/src/framework/Makefile .  >> $LOGFILE
	make >> $LOGFILE
	make install >> $LOGFILE
	echo "Successfully built and installed Starsphere [Framework]!" | tee -a $LOGFILE

	echo "Building Starsphere [Application]..." | tee -a $LOGFILE
	export STARSPHERE_SRC=$ROOT/src/starsphere
	export STARSPHERE_INSTALL=$ROOT/install
	cd $ROOT/build/starsphere
	cp $ROOT/src/starsphere/Makefile .  >> $LOGFILE
	cp $ROOT/src/starsphere/*.res .  >> $LOGFILE
	make >> $LOGFILE
	make install >> $LOGFILE
	echo "Successfully built and installed Starsphere [Application]!" | tee -a $LOGFILE

	return 0
}


build_linux()
{
	build_generic
	build_starsphere

	return 0
}


build_mac()
{
	echo "Not yet implemented: build_mac()"

# 	build_generic
# 	build_starsphere

	return 0
}


build_win32()
{
	echo "Not yet implemented: build_win32()"

# 	build_mingw
# 	build_generic
# 	build_starsphere

	return 0
}


distclean()
{
	cd $ROOT

	rm -rf 3rdparty
	rm -rf build
	rm -rf install

	rm -f .lastbuild
}


check_last_build()
{
	LASTBUILD=`cat .lastbuild 2>/dev/null`

	if [[ ( -f .lastbuild ) && ( "$LASTBUILD" != "$1" ) ]]; then
		cd $ROOT
		rm -rf build
		rm -rf install
	fi

	echo "$1" > .lastbuild

	return 0
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

	echo "Wrong usage. Stopping!" >> $LOGFILE

	return 0
}


### main control ##########################################################

TARGET_LINUX=1
TARGET_MAC=2
TARGET_WIN32=3

echo "************************************" >> $LOGFILE
echo "Starting new setup run!" >> $LOGFILE
echo "$DATE" >> $LOGFILE
echo "************************************" >> $LOGFILE

# crude command line parsing :-)

if [ $# -ne 1 ]; then
  print_usage
  exit 1
fi

case "$1" in
	"--linux")
		TARGET=$TARGET_LINUX
		check_last_build "$1"
		echo "Building linux version:" | tee -a $LOGFILE
		;;
	"--mac")
		TARGET=$TARGET_MAC
		check_last_build "$1"
		echo "Building mac version:" | tee -a $LOGFILE
		;;
	"--win32")
		TARGET=$TARGET_WIN32
		check_last_build "$1"
		echo "Building win32 version:" | tee -a $LOGFILE
		;;
	"--distclean")
		distclean
		exit 0
		;;
	*)
		print_usage
		exit 1
		;;
esac

# here we go...

check_prerequisites
if [ ! $? -eq 0 ]; then
	exit 1
fi

prepare_generic
if [ ! $? -eq 0 ]; then
	exit 1
fi

case $TARGET in
	$TARGET_LINUX)
		build_linux
		if [ ! $? -eq 0 ]; then
			exit 1
		fi
		;;
	$TARGET_MAC)
		build_mac
		if [ ! $? -eq 0 ]; then
			exit 1
		fi
		;;
	$TARGET_WIN32)
		build_win32
		if [ ! $? -eq 0 ]; then
			exit 1
		fi
		;;
	*)
		# should be unreachable
		print_usage
		exit 1
		;;
esac

exit 0
