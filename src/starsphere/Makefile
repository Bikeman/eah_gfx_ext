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

# path settings
STARSPHERE_SRC?=$(PWD)
STARSPHERE_INSTALL?=$(PWD)

# config values
CXX ?= g++

# variables
LIBS = -Wl,-Bstatic -lframework -loglft -L$(STARSPHERE_INSTALL)/lib
LIBS += $(shell $(STARSPHERE_INSTALL)/bin/freetype-config --libs)
LIBS += -lboinc_api -lboinc -L/usr/lib
LIBS += -Wl,-Bdynamic $(shell $(STARSPHERE_INSTALL)/bin/sdl-config --static-libs) -lGL -lGLU

CPPFLAGS = -I$(STARSPHERE_INSTALL)/include
CPPFLAGS += $(shell $(STARSPHERE_INSTALL)/bin/sdl-config --cflags)
CPPFLAGS += $(shell $(STARSPHERE_INSTALL)/bin/freetype-config --cflags)
CPPFLAGS += -I$(STARSPHERE_INSTALL)/include/BOINC -I/usr/include

DEPS = Makefile
OBJS = Starsphere.o StarsphereS5R3.o EinsteinS5R3Adapter.o starlist.o snr_list.o pulsar_list.o $(RESOURCESPEC).o
DEBUGFLAGSCPP = -pg -ggdb -O0
RESOURCESPEC = resources

# TODO: GraphicsEngineFactory obviously depends on the actual implementations (here starsphere)! need to change the structure! what about plugins?
CPPFLAGS += -I$(STARSPHERE_SRC) -I$(STARSPHERE_SRC)/../framework

# primary role based tagets
default: release
debug: starsphere
memcheck: clean debug starsphere
callgrind: clean debug starsphere
release: clean starsphere

# target specific options
debug: CPPFLAGS += $(DEBUGFLAGSCPP)
memcheck: CPPFLAGS += $(DEBUGFLAGSCPP) -DDEBUG_VALGRIND
callgrind: CPPFLAGS += $(DEBUGFLAGSCPP) -DDEBUG_VALGRIND
release: CPPFLAGS += -DNDEBUG -O3 -Wall -Wno-switch-enum
release: LDFLAGS += -s

# file based targets
starsphere: $(DEPS) $(STARSPHERE_SRC)/main.cpp $(OBJS)
	$(CXX) -g $(CPPFLAGS) $(LDFLAGS) $(STARSPHERE_SRC)/main.cpp -o starsphere $(OBJS) $(LIBS)

Starsphere.o: $(DEPS) $(STARSPHERE_SRC)/Starsphere.cpp $(STARSPHERE_SRC)/Starsphere.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/Starsphere.cpp
	
StarsphereS5R3.o: $(DEPS) $(STARSPHERE_SRC)/StarsphereS5R3.cpp $(STARSPHERE_SRC)/StarsphereS5R3.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/StarsphereS5R3.cpp

EinsteinS5R3Adapter.o: Makefile $(STARSPHERE_SRC)/EinsteinS5R3Adapter.cpp $(STARSPHERE_SRC)/EinsteinS5R3Adapter.h
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/EinsteinS5R3Adapter.cpp

starlist.o: $(DEPS) $(STARSPHERE_SRC)/starlist.C
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/starlist.C

snr_list.o: $(DEPS) $(STARSPHERE_SRC)/snr_list.C
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/snr_list.C

pulsar_list.o: $(DEPS) $(STARSPHERE_SRC)/pulsar_list.C
	$(CXX) -g $(CPPFLAGS) -c $(STARSPHERE_SRC)/pulsar_list.C

# resource compiler
$(RESOURCESPEC).o: $(STARSPHERE_SRC)/$(RESOURCESPEC).orc
	$(STARSPHERE_INSTALL)/bin/orc $(STARSPHERE_SRC)/$(RESOURCESPEC).orc $(RESOURCESPEC).cpp
	$(CXX) -g $(CPPFLAGS) -c $(RESOURCESPEC).cpp -o $(RESOURCESPEC).o


# tools
memcheck:
	valgrind --tool=memcheck --track-fds=yes --time-stamp=yes --log-file=$(PWD)/memcheck.out.%p --leak-check=full $(PWD)/starsphere

callgrind:
	valgrind --tool=callgrind --track-fds=yes --time-stamp=yes $(PWD)/starsphere

install:
	cp starsphere $(STARSPHERE_INSTALL)/bin

clean:
	rm -f $(RESOURCESPEC).cpp $(OBJS) starsphere
