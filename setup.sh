#!/bin/sh
ROOT=`pwd`
DATE=`date`

echo "************************************" >> setup.log
echo "Starting new setup run!" >> setup.log
echo "$DATE" >> setup.log
echo "************************************" >> setup.log

# prepare tree structure

echo "Preparing source tree..." | tee -a setup.log
mkdir -p lib/sdl >> setup.log
mkdir -p lib/freetype2 >> setup.log
mkdir -p lib/oglft >> setup.log
mkdir -p lib/boinc >> setup.log

echo "Preparing build tree..." | tee -a setup.log
mkdir -p build/sdl >> setup.log
mkdir -p build/freetype2 >> setup.log
mkdir -p build/oglft >> setup.log
mkdir -p build/boinc >> setup.log

echo "Preparing install tree..." | tee -a setup.log
mkdir -p install/sdl >> setup.log
mkdir -p install/freetype2 >> setup.log
mkdir -p install/oglft >> setup.log
mkdir -p install/boinc >> setup.log

# prepare additional sources

cd $ROOT
cd lib/sdl
if [ -d .svn ]; then
	echo "Updating SDL..." | tee -a ../../setup.log
	svn update >> ../../setup.log
else
	echo "Retrieving SDL..." | tee -a ../../setup.log
	svn checkout http://svn.libsdl.org/branches/SDL-1.2 . >> ../../setup.log
fi

cd $ROOT
cd lib/freetype2
if [ -d CVS ]; then
	echo "Updating Freetype2..." | tee -a ../../setup.log
	cvs update >> ../../setup.log 2>&1
else
	cd ..
	echo "Retrieving Freetype2..." | tee -a ../setup.log
	cvs -d:pserver:anonymous@cvs.sv.gnu.org:/sources/freetype checkout -r VER-2-3-5-REAL freetype2 >> ../setup.log 2>&1
fi

cd $ROOT
cd lib/oglft
if [ -d .svn ]; then
	echo "Updating OGLFT..." | tee -a ../../setup.log
	svn update >> ../../setup.log
else
	echo "Retrieving OGLFT..." | tee -a ../../setup.log
	svn checkout https://oglft.svn.sourceforge.net/svnroot/oglft/trunk . >> ../../setup.log
fi

cd $ROOT
cd lib/boinc
if [ -d .svn ]; then
	echo "Updating BOINC..." | tee -a ../../setup.log
	svn update >> ../../setup.log
else
	echo "Retrieving BOINC..." | tee -a ../../setup.log
	svn checkout http://boinc.berkeley.edu/svn/trunk/boinc . >> ../../setup.log
fi

# build additional sources

