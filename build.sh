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

# more refactoring (every lib in dedicated function? per traget?)

### globals ###############################################################

ROOT=`pwd`
LOGFILE=$ROOT/build.log
TARGET=0

### functions #############################################################

failure()
{
	echo "************************************" | tee -a $LOGFILE
	echo "Error detected! Stopping build!" | tee -a $LOGFILE
	echo "`date`" | tee -a $LOGFILE

    if [ -f "$LOGFILE" ]; then
		echo "------------------------------------"
		echo "Please check logfile: `basename $LOGFILE`"
		echo "These are the final ten lines:"
		echo "------------------------------------"
		tail -n 14 $LOGFILE | head -n 10
    fi

	echo "************************************" | tee -a $LOGFILE

    exit 1
}

check_prerequisites()
{
	echo "Checking prerequisites..." | tee -a $LOGFILE
	
	# required toolchain
	TOOLS="automake autoconf m4 cmake wget gcc g++ ld libtool ar lex yacc"

	for tool in $TOOLS; do
		if ! ( type $tool >/dev/null 2>&1 ); then
			echo "Missing \"$tool\" which is a required tool!" | tee -a $LOGFILE
			return 1
		fi
	done

	return 0
}


prepare_source_tree()
{
	echo "Preparing source tree..." | tee -a $LOGFILE
	mkdir -p 3rdparty/oglft >> $LOGFILE || failure
	mkdir -p 3rdparty/boinc >> $LOGFILE || failure

	return 0
}


prepare_build_tree()
{
	echo "Preparing build tree..." | tee -a $LOGFILE
	mkdir -p build/sdl >> $LOGFILE || failure
	mkdir -p build/freetype2 >> $LOGFILE || failure
	mkdir -p build/oglft >> $LOGFILE || failure
	mkdir -p build/boinc >> $LOGFILE || failure
	mkdir -p build/framework >> $LOGFILE || failure
	mkdir -p build/orc >> $LOGFILE || failure
	mkdir -p build/starsphere >> $LOGFILE || failure

	return 0
}


prepare_install_tree()
{
	echo "Preparing install tree..." | tee -a $LOGFILE
	mkdir -p install/bin >> $LOGFILE || failure
	mkdir -p install/include >> $LOGFILE || failure
	mkdir -p install/lib >> $LOGFILE || failure

	return 0
}


prepare_generic()
{
	prepare_source_tree || failure
	prepare_build_tree || failure
	prepare_install_tree || failure

	# prepare additional sources

	cd $ROOT || failure
	mkdir -p 3rdparty/sdl >> $LOGFILE 2>&1 || failure
	cd $ROOT/3rdparty/sdl || failure
	if [ -d .svn ]; then
		echo "Updating SDL..." | tee -a $LOGFIL
		# make sure local changes (patches) are reverted, hence also updated
		svn revert -R . >> $LOGFILE 2>&1 || failure
		svn update >> $LOGFILE 2>&1 || failure
	else
		echo "Retrieving SDL (this may take a while)..." | tee -a $LOGFILE
		svn checkout http://svn.libsdl.org/branches/SDL-1.2 . >> $LOGFILE 2>&1 || failure
	fi

# 	cd $ROOT/3rdparty || failure
# 	echo "Retrieving SDL (this may take a while)..." | tee -a $LOGFILE
# 	wget http://www.libsdl.org/release/SDL-1.2.14.tar.gz >> $LOGFILE 2>&1 || failure
# 	tar -xzf SDL-1.2.14.tar.gz >> $LOGFILE 2>&1 || failure
# 	rm SDL-1.2.14.tar.gz >> $LOGFILE 2>&1 || failure
# 	substitute old source tree
# 	rm -rf sdl >> $LOGFILE 2>&1 || failure
# 	mv SDL-1.2.14 sdl >> $LOGFILE 2>&1 || failure
	
	cd $ROOT/3rdparty || failure
	echo "Retrieving Freetype2 (this may take a while)..." | tee -a $LOGFILE
	wget http://mesh.dl.sourceforge.net/sourceforge/freetype/freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
	tar -xjf freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
	rm freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
	# substitute old source tree
	rm -rf freetype2 >> $LOGFILE 2>&1 || failure
	mv freetype-2.3.5 freetype2 >> $LOGFILE 2>&1 || failure
	
	cd $ROOT/3rdparty/oglft || failure
	if [ -d .svn ]; then
		echo "Updating OGLFT..." | tee -a $LOGFILE
		# make sure local changes (patches) are reverted, hence also updated
		svn revert -R . >> $LOGFILE  2>&1 || failure
		svn update >> $LOGFILE  2>&1 || failure
	else
		echo "Retrieving OGLFT (this may take a while)..." | tee -a $LOGFILE
		svn checkout https://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> $LOGFILE 2>&1 || failure
	fi
	
	cd $ROOT/3rdparty/boinc || failure
	if [ -d .svn ]; then
		echo "Updating BOINC..." | tee -a $LOGFILE
		# make sure local changes (patches) are reverted, hence also updated
		svn revert -R . >> $LOGFILE  2>&1 || failure
		svn update >> $LOGFILE  2>&1 || failure
	else
		echo "Retrieving BOINC (this may take a while)..." | tee -a $LOGFILE
		svn checkout http://boinc.berkeley.edu/svn/trunk/boinc . >> $LOGFILE 2>&1 || failure
	fi

	return 0
}


prepare_win32()
{
	cd $ROOT || failure

	echo "Preparing MinGW source tree..." | tee -a $LOGFILE
	mkdir -p 3rdparty/mingw/xscripts >> $LOGFILE || failure
	cd 3rdparty/mingw/xscripts || failure

	if [ -d CVS ]; then
		echo "Updating MinGW build script..." | tee -a $LOGFILE
		cvs update -C >> $LOGFILE 2>&1 || failure
	else
		cd .. || failure
		echo "Retrieving MinGW build script (this may take a while)..." | tee -a $LOGFILE
		cvs -z3 -d:pserver:anonymous@mingw.cvs.sourceforge.net:/cvsroot/mingw checkout -P xscripts >> $LOGFILE 2>&1 || failure
	fi
	
	cd $ROOT/3rdparty/mingw/xscripts || failure
	echo "Preparing MinGW build script..." | tee -a $LOGFILE
	# note: svn has no force/overwrite switch. the file might not be updated when patched
	patch x86-mingw32-build.sh.conf < $ROOT/patches/x86-mingw32-build.sh.conf.patch >> $LOGFILE || failure
	chmod +x x86-mingw32-build.sh >> $LOGFILE || failure

	return 0
}


build_generic()
{
	cd $ROOT/3rdparty/sdl || failure
	echo "Building SDL (this may take a while)..." | tee -a $LOGFILE
	./autogen.sh >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/sdl || failure
	$ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed SDL!" | tee -a $LOGFILE

	cd $ROOT/3rdparty/freetype2 || failure
	echo "Building Freetype2 (this may take a while)..." | tee -a $LOGFILE
	chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	./autogen.sh >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/freetype2 || failure
	# note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
	$ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Freetype2!" | tee -a $LOGFILE

	cd $ROOT/3rdparty/oglft || failure
	echo "Patching OGLFT..." | tee -a $LOGFILE
	# note: svn has no force/overwrite switch. patched files might not be updated
	# patch: use fixed settings for freetype, deactivate FindFreetype
	FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
	FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
	# patch: build static lib instead of shared
	cd $ROOT/3rdparty/oglft/liboglft || failure
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.liboglft.patch >> $LOGFILE 2>&1 || failure
	if [ "$1" == "$TARGET_MAC" ]; then
		# patch: enable Mac OS support
		patch OGLFT.h.cmake < $ROOT/patches/OGLFT.h.cmake.macos.patch >> $LOGFILE 2>&1 || failure
	fi
	echo "Building OGLFT..." | tee -a $LOGFILE
	cd $ROOT/build/oglft || failure
	# TODO: do we wanna create universal binaries on mac? If so, add -DCMAKE_OSX_ARCHITECTURES=ppc;i386
	cmake -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
	cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
	cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed OGLFT!" | tee -a $LOGFILE

	cd $ROOT/3rdparty/boinc || failure
	echo "Building BOINC (this may take a while)..." | tee -a $LOGFILE
	./_autosetup >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/boinc || failure
	$ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
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
	export ORC_SRC=$ROOT/src/orc || failure
	export ORC_INSTALL=$ROOT/install || failure
	cd $ROOT/build/orc || failure
	cp $ROOT/src/orc/Makefile . >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Starsphere [ORC]!" | tee -a $LOGFILE

	echo "Building Starsphere [Framework]..." | tee -a $LOGFILE
	export FRAMEWORK_SRC=$ROOT/src/framework || failure
	export FRAMEWORK_INSTALL=$ROOT/install || failure
	cd $ROOT/build/framework || failure
	cp $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Starsphere [Framework]!" | tee -a $LOGFILE

	echo "Building Starsphere [Application]..." | tee -a $LOGFILE
	export STARSPHERE_SRC=$ROOT/src/starsphere || failure
	export STARSPHERE_INSTALL=$ROOT/install || failure
	cd $ROOT/build/starsphere || failure
	cp $ROOT/src/starsphere/*.res . >> $LOGFILE 2>&1 || failure
	if [ "$1" != "$TARGET_MAC" ]; then
		cp $ROOT/src/starsphere/Makefile . >> $LOGFILE 2>&1 || failure
	else
		cp $ROOT/src/starsphere/Makefile.macos Makefile >> $LOGFILE 2>&1 || failure
	fi
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Starsphere [Application]!" | tee -a $LOGFILE

	return 0
}


build_linux()
{
	build_generic || failure
	build_starsphere || failure

	return 0
}


build_mac()
{
	build_generic $TARGET_MAC || failure
	build_starsphere $TARGET_MAC || failure

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
	cd $ROOT || failure

	echo "Purging build system..." | tee -a $LOGFILE

	rm -rf 3rdparty || failure
	rm -rf build || failure
	rm -rf install || failure

	rm -f .lastbuild || failure
}


check_last_build()
{
	echo "Checking previous build target..." | tee -a $LOGFILE

	LASTBUILD=`cat .lastbuild 2>/dev/null`

	if [[ ( -f .lastbuild ) && ( "$LASTBUILD" != "$1" ) ]]; then
		cd $ROOT || failure
		echo "Build target changed! Purging build and install trees..." | tee -a $LOGFILE
		rm -rf build >> $LOGFILE || failure
		rm -rf install >> $LOGFILE || failure
	fi

	echo "$1" > .lastbuild || failure

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

echo "************************************" | tee -a $LOGFILE
echo "Starting new build!" | tee -a $LOGFILE
echo "`date`" | tee -a $LOGFILE
echo "************************************" | tee -a $LOGFILE

# crude command line parsing :-)

if [ $# -ne 1 ]; then
  print_usage
  exit 1
fi

case "$1" in
	"--linux")
		TARGET=$TARGET_LINUX
		check_last_build "$1" || failure
		echo "Building linux version:" | tee -a $LOGFILE
		;;
	"--mac")
		TARGET=$TARGET_MAC
		check_last_build "$1" || failure
		echo "Building mac version:" | tee -a $LOGFILE
		;;
	"--win32")
		TARGET=$TARGET_WIN32
		check_last_build "$1" || failure
		echo "Building win32 version:" | tee -a $LOGFILE
		;;
	"--distclean")
		distclean || failure
		exit 0
		;;
	*)
		print_usage
		exit 1
		;;
esac

# here we go...

check_prerequisites || failure
prepare_generic || failure

case $TARGET in
	$TARGET_LINUX)
		build_linux || failure
		;;
	$TARGET_MAC)
		build_mac || failure
		;;
	$TARGET_WIN32)
		build_win32 || failure
		;;
	*)
		# should be unreachable
		print_usage
		exit 1
		;;
esac

echo "************************************" | tee -a $LOGFILE
echo "Build finished successfully!" | tee -a $LOGFILE
echo "`date`" | tee -a $LOGFILE
echo "************************************" | tee -a $LOGFILE

exit 0
