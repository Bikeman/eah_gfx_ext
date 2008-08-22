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
ORC_SRC?=$(PWD)
ORC_INSTALL?=$(PWD)

# config values
CXX?=g++

# variables
CPPFLAGS += -I/usr/include
LDFLAGS +=
LIBS += -L/usr/lib

OBJS=ResourceCompiler.o
DEPS=Makefile

# primary role based tagets
default: release
debug: orc
release: clean orc

# target specific options
debug: CPPFLAGS += -pg -ggdb3 -O0 -Wall -Wno-switch-enum
release: CPPFLAGS += -DNDEBUG -ggdb3 -O3 -Wall -Wno-switch-enum
release: LDFLAGS +=

# file specific targets
orc: $(DEPS) $(ORC_SRC)/orc.cpp $(OBJS)
	$(CXX) -g $(CPPFLAGS) $(LDFLAGS) $(ORC_SRC)/orc.cpp -o orc $(OBJS) $(LIBS)

ResourceCompiler.o: $(DEPS) $(ORC_SRC)/ResourceCompiler.cpp $(ORC_SRC)/ResourceCompiler.h
	$(CXX) -g $(CPPFLAGS) -c $(ORC_SRC)/ResourceCompiler.cpp

install:
	cp orc $(ORC_INSTALL)/bin

clean:
	rm -f $(OBJS) orc
