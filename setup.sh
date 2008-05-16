#!/bin/sh

### functions #######################################################

check_prerequisites()
{
	echo "Not yet implemented: check_prerequisites()"
	# wget
	# C compiler
	# lex or flex
	# yacc or bison or byacc
	# m4
}


prepare_generic()
{
	echo "Preparing source tree..." | tee -a setup.log
	mkdir -p 3rdparty/sdl >> setup.log
	mkdir -p 3rdparty/freetype2 >> setup.log
	mkdir -p 3rdparty/oglft >> setup.log
	mkdir -p 3rdparty/boinc >> setup.log
	
	echo "Preparing build tree..." | tee -a setup.log
	mkdir -p build/sdl >> setup.log
	mkdir -p build/freetype2 >> setup.log
	mkdir -p build/oglft >> setup.log
	mkdir -p build/boinc >> setup.log
	
	echo "Preparing install tree..." | tee -a setup.log
	mkdir -p install/bin >> setup.log
	mkdir -p install/include >> setup.log
	mkdir -p install/lib >> setup.log
	
	# prepare additional sources
	
	cd $ROOT
	cd 3rdparty/sdl
	if [ -d .svn ]; then
		echo "Updating SDL..." | tee -a ../../setup.log
		svn update >> ../../setup.log
	else
		echo "Retrieving SDL..." | tee -a ../../setup.log
		svn checkout http://svn.libsdl.org/branches/SDL-1.2 . >> ../../setup.log
	fi
	
	cd $ROOT
	cd 3rdparty/freetype2
	if [ -d CVS ]; then
		echo "Updating Freetype2..." | tee -a ../../setup.log
		cvs update >> ../../setup.log 2>&1
	else
		cd ..
		echo "Retrieving Freetype2..." | tee -a ../setup.log
		cvs -z3 -d:pserver:anonymous@cvs.sv.gnu.org:/sources/freetype checkout -r VER-2-3-5-REAL freetype2 >> ../setup.log 2>&1
	fi
	
	cd $ROOT
	cd 3rdparty/oglft
	if [ -d .svn ]; then
		echo "Updating OGLFT..." | tee -a ../../setup.log
		svn update >> ../../setup.log
	else
		echo "Retrieving OGLFT..." | tee -a ../../setup.log
		svn checkout https://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> ../../setup.log
	fi
	
	cd $ROOT
	cd 3rdparty/boinc
	if [ -d .svn ]; then
		echo "Updating BOINC..." | tee -a ../../setup.log
		svn update >> ../../setup.log
	else
		echo "Retrieving BOINC..." | tee -a ../../setup.log
		svn checkout http://boinc.berkeley.edu/svn/trunk/boinc . >> ../../setup.log
	fi
}


prepare_win32()
{
	cd $ROOT
	mkdir -p 3rdparty/mingw/xscripts >> setup.log
	cd 3rdparty/mingw/xscripts

	if [ -d CVS ]; then
		echo "Updating MinGW build script..." | tee -a ../../../setup.log
		cvs update -C >> ../../../setup.log 2>&1
	else
		cd ..
		echo "Retrieving MinGW build script..." | tee -a ../../setup.log
		cvs -z3 -d:pserver:anonymous@mingw.cvs.sourceforge.net:/cvsroot/mingw checkout -P xscripts >> ../../setup.log 2>&1
	fi
	
	cd $ROOT
	cd 3rdparty/mingw/xscripts
	echo "Preparing MinGW build script..." | tee -a ../../../setup.log
	cp -f $ROOT/patches/x86-mingw32-build.sh.conf.patch . >> ../../../setup.log
	patch x86-mingw32-build.sh.conf < x86-mingw32-build.sh.conf.patch >> ../../../setup.log
	chmod +x x86-mingw32-build.sh >> ../../../setup.log
}


build_generic()
{
	echo "Not yet implemented: build_generic()"
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


build_mingw()
{
	echo "Not yet implemented: build_mingw()"
}


build_starsphere()
{
	echo "Not yet implemented: build_starsphere()"
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

	echo "Wrong usage. Stopping!" >> setup.log
}


### main control ####################################################

ROOT=`pwd`
DATE=`date`

TARGET=0
TARGET_LINUX=1
TARGET_MAC=2
TARGET_WIN32=3

echo "************************************" >> setup.log
echo "Starting new setup run!" >> setup.log
echo "$DATE" >> setup.log
echo "************************************" >> setup.log

# crude command line parsing :-)

if [ $# -ne 1 ]; then
  print_usage
  exit 1
fi

case "$1" in
	"--linux")
		TARGET=$TARGET_LINUX
		check_last_build "$1"
		echo "Building linux version:" | tee -a setup.log
		;;
	"--mac")
		TARGET=$TARGET_MAC
		check_last_build "$1"
		echo "Building mac version:" | tee -a setup.log
		;;
	"--win32")
			TARGET=$TARGET_WIN32
			check_last_build "$1"
			echo "Building win32 version:" | tee -a setup.log
			;;
	*)
			print_usage
			exit 1
			;;
esac

# here we go...

check_prerequisites
prepare_generic

exit 0
