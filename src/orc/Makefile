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

# config values
CXX?=g++
DEBUGFLAGSCPP=-DDEBUG -pg -ggdb -O0
OBJS=ResourceCompiler.o orc.o
DEPS=Makefile ResourceCompiler.h

# primary role based tagets
default: release
debug: orc
release: clean orc

# target specific options
debug: CPPFLAGS += $(DEBUGFLAGSCPP)
release: CPPFLAGS += -O3 -Wall -Wno-switch-enum
release: LDFLAGS += -s

# file specific targets
orc: $(DEPS) ResourceCompiler.cpp orc.cpp ${OBJS}
	$(CXX) -g ${CPPFLAGS} ResourceCompiler.cpp orc.cpp -o orc

orc.o: $(DEPS) orc.cpp
	$(CXX) -g ${CPPFLAGS} -c orc.cpp

ResourceCompiler.o: $(DEPS) ResourceCompiler.cpp
	$(CXX) -g ${CPPFLAGS} -c ResourceCompiler.cpp

clean:
	rm -f ${OBJS} orc