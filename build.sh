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
PATH_ORG="$PATH"
PATH_MINGW="$PATH"
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
	TOOLS="automake autoconf m4 cmake wget svn cvs tar gcc g++ ld libtool ar lex yacc"

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
	mkdir -p build/libxml2 >> $LOGFILE || failure
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
	
	echo "Retrieving Freetype2 (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty || failure
	wget http://mesh.dl.sourceforge.net/sourceforge/freetype/freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
	tar -xjf freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
	rm freetype-2.3.5.tar.bz2 >> $LOGFILE 2>&1 || failure
	# substitute old source tree
	rm -rf freetype2 >> $LOGFILE 2>&1 || failure
	mv freetype-2.3.5 freetype2 >> $LOGFILE 2>&1 || failure

	echo "Retrieving libxml2 (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty || failure
	wget --passive-ftp ftp://xmlsoft.org/libxml2/libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
	tar -xzf libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
	rm libxml2-sources-2.6.32.tar.gz >> $LOGFILE 2>&1 || failure
	# substitute old source tree
	rm -rf libxml2 >> $LOGFILE 2>&1 || failure
	mv libxml2-2.6.32 libxml2 >> $LOGFILE 2>&1 || failure
	
	cd $ROOT/3rdparty/oglft || failure
	if [ -d .svn ]; then
		echo "Updating OGLFT..." | tee -a $LOGFILE
		# make sure local changes (patches) are reverted, hence also updated
		svn revert -R . >> $LOGFILE  2>&1 || failure
		svn update >> $LOGFILE  2>&1 || failure
	else
		echo "Retrieving OGLFT (this may take a while)..." | tee -a $LOGFILE
		svn checkout http://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> $LOGFILE 2>&1 || failure
	fi
	
	cd $ROOT/3rdparty/boinc || failure
	if [ -d .svn ]; then
		echo "Updating BOINC..." | tee -a $LOGFILE
		# make sure local changes (patches) are reverted, hence also updated
		svn revert -R . >> $LOGFILE  2>&1 || failure
		svn update >> $LOGFILE  2>&1 || failure
	else
		echo "Retrieving BOINC (this may take a while)..." | tee -a $LOGFILE
		svn checkout http://boinc.berkeley.edu/svn/branches/boinc_core_release_6_2 . >> $LOGFILE 2>&1 || failure
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
	
	echo "Preparing MinGW build script..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/mingw/xscripts || failure
	# note: svn has no force/overwrite switch. the file might not be updated when patched
	patch x86-mingw32-build.sh.conf < $ROOT/patches/x86-mingw32-build.sh.conf.patch >> $LOGFILE || failure
	chmod +x x86-mingw32-build.sh >> $LOGFILE || failure

	return 0
}


build_generic()
{
	echo "Building SDL (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/sdl || failure
	chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
	./autogen.sh >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/sdl || failure
	if [ "$1" == "$TARGET_MAC_INTEL" -o "$1" == "$TARGET_MAC_PPC" ]; then
		$ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --enable-screensaver=yes --enable-video-x11=no >> $LOGFILE 2>&1 || failure
	else
		$ROOT/3rdparty/sdl/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --enable-screensaver=yes >> $LOGFILE 2>&1 || failure	
	fi
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed SDL!" | tee -a $LOGFILE

	echo "Building Freetype2 (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/freetype2 || failure
	chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
	./autogen.sh >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/freetype2 || failure
	# note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
	$ROOT/3rdparty/freetype2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Freetype2!" | tee -a $LOGFILE

	echo "Building libxml2 (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/libxml2 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/libxml2 || failure
	$ROOT/3rdparty/libxml2/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed libxml2!" | tee -a $LOGFILE

	echo "Patching OGLFT..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/oglft || failure
	# note: svn has no force/overwrite switch. patched files might not be updated
	# patch: use fixed settings for freetype, deactivate FindFreetype
	FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
	FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
	echo "Building OGLFT..." | tee -a $LOGFILE
	cd $ROOT/build/oglft || failure
	# TODO: do we wanna create universal binaries on mac? If so, add -DCMAKE_OSX_ARCHITECTURES=ppc;i386
	cmake -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
	cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
	cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed OGLFT!" | tee -a $LOGFILE

	echo "Building BOINC (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/boinc || failure
	chmod +x _autosetup >> $LOGFILE 2>&1 || failure
	./_autosetup >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/boinc || failure
	if [ "$1" == "$TARGET_MAC_INTEL" -o "$1" == "$TARGET_MAC_PPC" ]; then
		export CPPFLAGS=-I/sw/include
		$ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client --with-apple-opengl-framework >> $LOGFILE 2>&1 || failure
	else
		$ROOT/3rdparty/boinc/configure --prefix=$ROOT/install --enable-shared=no --enable-static=yes --disable-server --disable-client >> $LOGFILE 2>&1 || failure
	fi
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed BOINC!" | tee -a $LOGFILE

	return 0
}


build_generic_win32()
{
	# general config
	PREFIX=$ROOT/install
	TARGET_HOST=i586-pc-mingw32
	BUILD_HOST=i386-linux
	PATH_MINGW="$PREFIX/bin:$PREFIX/$TARGET_HOST/bin:$PATH"
	PATH="$PATH_MINGW"
	export PATH

	echo "Building SDL (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/sdl || failure
	chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
	./autogen.sh >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-sdl-config" ]; then
		SDL_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-sdl-config"
		export SDL_CONFIG
		echo "Cross-compile SDL_CONFIG: $SDL_CONFIG" >> $LOGFILE
	fi
	cd $ROOT/build/sdl || failure
	$ROOT/3rdparty/sdl/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes --enable-screensaver=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed SDL!" | tee -a $LOGFILE

	echo "Patching Freetype2..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/freetype2/builds || failure
	# patch: deactivating invocation of apinames (would run win32 binary on linux host)
	patch < $ROOT/patches/freetype2.exports.mk.patch >> $LOGFILE 2>&1 || failure
	echo "Building Freetype2 (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/freetype2 || failure
	chmod +x autogen.sh >> $LOGFILE 2>&1 || failure
	./autogen.sh >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-freetype-config" ]; then
		FT_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-freetype-config"
		export FT_CONFIG
		echo "Cross-compile FT_CONFIG: $FT_CONFIG" >> $LOGFILE
	fi
	cd $ROOT/build/freetype2 || failure
	# note: freetype (or sdl?) probably doesn't need *no* configure when static -> ansi build, see readme!
	$ROOT/3rdparty/freetype2/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Freetype2!" | tee -a $LOGFILE

	echo "Building libxml2 (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/libxml2 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	if [ -f "$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-xml2-config" ]; then
		LIBXML2_CONFIG="$PREFIX/$TARGET_HOST/bin/$TARGET_HOST-xml2-config"
		export LIBXML2_CONFIG
		echo "Cross-compile LIBXML2_CONFIG: $LIBXML2_CONFIG" >> $LOGFILE
	fi
	cd $ROOT/build/libxml2 || failure
	$ROOT/3rdparty/libxml2/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$PREFIX --enable-shared=no --enable-static=yes >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed libxml2!" | tee -a $LOGFILE

	echo "Patching OGLFT..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/oglft || failure
	# note: svn has no force/overwrite switch. patched files might not be updated
	# patch: use fixed settings for freetype, deactivate FindFreetype
	FREETYPE2_INCLUDE_DIR="$ROOT/install/include"
	FREETYPE2_LIBRARIES="$ROOT/install/lib/libfreetype.a"
	patch CMakeLists.txt < $ROOT/patches/CMakeLists.txt.oglft.patch >> $LOGFILE 2>&1 || failure
	cp $ROOT/patches/toolchain-linux-mingw.oglft.cmake $ROOT/build/oglft >> $LOGFILE 2>&1 || failure
	export OGLFT_INSTALL=$ROOT/install
	echo "Building OGLFT..." | tee -a $LOGFILE
	cd $ROOT/build/oglft || failure
	cmake -DCMAKE_TOOLCHAIN_FILE="toolchain-linux-mingw.oglft.cmake" -DFREETYPE2_INCLUDE_DIR="$FREETYPE2_INCLUDE_DIR" -DFREETYPE2_LIBRARIES="$FREETYPE2_LIBRARIES" $ROOT/3rdparty/oglft >> $LOGFILE 2>&1 || failure
	make >> $LOGFILE 2>&1 || failure
	mkdir -p $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
	cp OGLFT.h $ROOT/install/include/oglft >> $LOGFILE 2>&1 || failure
	cp liboglft/liboglft.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed OGLFT!" | tee -a $LOGFILE

	echo "Patching BOINC..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/boinc/lib || failure
	# patch: fix a couple of BOINC vs. MinGW issues
	patch boinc_win.h < $ROOT/patches/boinc.boinc_win.h.minggw.patch >> $LOGFILE 2>&1 || failure
	patch filesys.C < $ROOT/patches/boinc.filesys.C.mingw.patch >> $LOGFILE 2>&1 || failure
	# patch: add graphics2 and customize build path (see below)
	echo "Building BOINC (this may take a while)..." | tee -a $LOGFILE
	cd $ROOT/3rdparty/boinc || failure
	chmod +x _autosetup >> $LOGFILE 2>&1 || failure
	./_autosetup >> $LOGFILE 2>&1 || failure
	chmod +x configure >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/boinc || failure
	# note: configure is still required but we don't use the generated Makefile
	$ROOT/3rdparty/boinc/configure --host=$TARGET_HOST --build=$BUILD_HOST --prefix=$ROOT/install --includedir=$ROOT/install/include --oldincludedir=$ROOT/install/include --enable-shared=no --enable-static=yes --disable-server --disable-client >> $LOGFILE 2>&1 || failure
	cd $ROOT/build/boinc/api || failure
	cp $ROOT/3rdparty/boinc/api/Makefile.mingw . >> $LOGFILE 2>&1 || failure
	patch Makefile.mingw < $ROOT/patches/boinc.Makefile.mingw.patch >> $LOGFILE 2>&1 || failure
	export BOINC_SRC=$ROOT/3rdparty/boinc || failure
	cd $ROOT/build/boinc || failure
	make -f api/Makefile.mingw >> $LOGFILE 2>&1 || failure
	cp $ROOT/build/boinc/libboinc.a $ROOT/install/lib >> $LOGFILE 2>&1 || failure
	mkdir -p $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/api/boinc_api.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/api/graphics2.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/app_ipc.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/boinc_win.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/common_defs.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/diagnostics.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/hostinfo.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/proxy_info.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/prefs.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/miofile.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/mfile.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/parse.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	cp $ROOT/3rdparty/boinc/lib/util.h $ROOT/install/include/BOINC >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed BOINC!" | tee -a $LOGFILE
}


build_mingw()
{
	TARGET_HOST=i586-pc-mingw32

	echo "Building MinGW (this will take quite a while)..." | tee -a $LOGFILE
	# note: the script's current config for unattended setup expects it to be run from three levels below root!
	cd $ROOT/3rdparty/mingw/xscripts || failure
	./x86-mingw32-build.sh --unattended $TARGET_HOST >> $LOGFILE 2>&1 || failure

	return 0
}


build_starsphere()
{
	# make sure ORC is always compiled for host platform (it's exexuted during starsphere build!)
	export PATH=$PATH_ORG

	echo "Building Starsphere [ORC]..." | tee -a $LOGFILE
	export ORC_SRC=$ROOT/src/orc || failure
	export ORC_INSTALL=$ROOT/install || failure
	cd $ROOT/build/orc || failure
	cp $ROOT/src/orc/Makefile . >> $LOGFILE 2>&1 || failure
	make $2 >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Starsphere [ORC]!" | tee -a $LOGFILE

	# set main include directory
	if [ "$1" == "$TARGET_WIN32" ]; then
		export PATH=$PATH_MINGW
	else
		export PATH=$PATH_ORG
	fi

	echo "Building Starsphere [Framework]..." | tee -a $LOGFILE
	export FRAMEWORK_SRC=$ROOT/src/framework || failure
	export FRAMEWORK_INSTALL=$ROOT/install || failure
	cd $ROOT/build/framework || failure
	if [ "$1" == "$TARGET_WIN32" ]; then
		cp -f $ROOT/src/framework/Makefile.mingw Makefile >> $LOGFILE 2>&1 || failure
	else
		cp -f $ROOT/src/framework/Makefile . >> $LOGFILE 2>&1 || failure
	fi
	make $2 >> $LOGFILE 2>&1 || failure
	make install >> $LOGFILE 2>&1 || failure
	echo "Successfully built and installed Starsphere [Framework]!" | tee -a $LOGFILE

	echo "Building Starsphere [Application]..." | tee -a $LOGFILE
	export STARSPHERE_SRC=$ROOT/src/starsphere || failure
	export STARSPHERE_INSTALL=$ROOT/install || failure
	cd $ROOT/build/starsphere || failure
	cp $ROOT/src/starsphere/*.res . >> $LOGFILE 2>&1 || failure
	if [ "$1" == "$TARGET_MAC_INTEL" ] || [ "$1" == "$TARGET_MAC_PPC" ]; then
		cp -f $ROOT/src/starsphere/Makefile.macos Makefile >> $LOGFILE 2>&1 || failure
	elif [ "$1" == "$TARGET_WIN32" ]; then
		cp -f $ROOT/src/starsphere/Makefile.mingw Makefile >> $LOGFILE 2>&1 || failure
	else
		cp -f $ROOT/src/starsphere/Makefile . >> $LOGFILE 2>&1 || failure
	fi
	make $2 >> $LOGFILE 2>&1 || failure
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
	build_generic $1 || failure
	build_starsphere $1 || failure

	return 0
}


build_win32()
{
	prepare_win32 || failure
	build_mingw || failure
	build_generic_win32 || failure
	build_starsphere $TARGET_WIN32 || failure

	return 0
}


distclean()
{
	cd $ROOT || failure

	echo "Purging build system..." | tee -a $LOGFILE

	rm -rf 3rdparty || failure
	rm -rf build || failure
	rm -rf install || failure
	rm -rf doc/html || failure
	rm -f doc/*.tag || failure

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
	echo "  --mac-intel"
	echo "  --mac-ppc"
	echo "  --win32"
	echo "  --doc"
	echo "*************************"

	echo "Wrong usage. Stopping!" >> $LOGFILE

	return 0
}


### main control ##########################################################

TARGET_LINUX=1
TARGET_MAC_INTEL=2
TARGET_MAC_PPC=4
TARGET_WIN32=8
TARGET_DOC=16

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
	"--mac-intel")
		TARGET=$TARGET_MAC_INTEL
		check_last_build "$1" || failure
		echo "Building mac (Intel) version:" | tee -a $LOGFILE
		;;
	"--mac-ppc")
		TARGET=$TARGET_MAC_PPC
		check_last_build "$1" || failure
		echo "Building mac (PPC) version:" | tee -a $LOGFILE
		;;
	"--win32")
		TARGET=$TARGET_WIN32
		check_last_build "$1" || failure
		echo "Building win32 version:" | tee -a $LOGFILE
		;;
	"--doc")
		TARGET=$TARGET_DOC
		echo "Building documentation..." | tee -a $LOGFILE
		;;
	"--distclean")
		distclean || failure
		exit 0
		;;
	"--starsphere")
		# "hidden" bonus option :-)
		TARGET=$TARGET_LINUX
		build_starsphere $TARGET "debug" || failure
		exit 0
		;;
	*)
		print_usage
		exit 1
		;;
esac

# here we go...

case $TARGET in
	$TARGET_LINUX)
		check_prerequisites || failure
		prepare_generic || failure
		build_linux || failure
		;;
	$TARGET_MAC_INTEL)
		check_prerequisites || failure
		prepare_generic || failure
		build_mac $TARGET_MAC_INTEL || failure
		;;
	$TARGET_MAC_PPC)
		export CFLAGS=-mcpu=G3 || failure
		export CXXFLAGS=-mcpu=G3 || failure
		check_prerequisites || failure
		prepare_generic || failure
		build_mac $TARGET_MAC_PPC || failure
		;;
	$TARGET_WIN32)
		check_prerequisites || failure
		prepare_generic || failure
		build_win32 || failure
		;;
	$TARGET_DOC)
		doxygen Doxyfile >> $LOGFILE 2>&1 || failure
		cp -f $ROOT/doc/default/*.png $ROOT/doc/html >> $LOGFILE 2>&1 || failure
		cp -f $ROOT/doc/default/*.gif $ROOT/doc/html >> $LOGFILE 2>&1 || failure
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
