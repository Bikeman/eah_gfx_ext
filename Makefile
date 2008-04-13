# point this to where you installed BOINC (--prefix you gave to configure, defaults to $BOINC_PREFIX)
BOINC_PREFIX ?= /home/oliver/development/aei/boinc
CXX ?= g++

# variables
LIBS = -L${BOINC_PREFIX}/lib -Bstatic $(shell sdl-config --static-libs) -lfreetype -lOGLFT -lboinc_api -lboinc -Wl,-Bdynamic -lGL -lGLU
CPPFLAGS = -DOGLFT_NO_SOLID -DOGLFT_NO_QT -I/usr/include $(shell sdl-config --cflags) -I/usr/include/freetype2 -I${BOINC_PREFIX}/include/BOINC
DEPS = Makefile
OBJS = starlist.o snr_list.o pulsar_list.o AbstractGraphicsEngine.o GraphicsEngineFactory.o Starsphere.o StarsphereS5R3.o WindowManager.o ${RESOURCESPEC}.o Resource.o ResourceFactory.o BOINCClientAdapter.o EinsteinS5R3Adapter.o
DEBUGFLAGSCPP = -pg -ggdb -O0
RESOURCESPEC = resources

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
starsphere: $(DEPS) main.C $(OBJS)
	$(CXX) -g ${CPPFLAGS} ${LDFLAGS} main.C -o starsphere ${OBJS} ${LIBS}

Starsphere.o: $(DEPS) Starsphere.cpp
	$(CXX) -g ${CPPFLAGS} -c Starsphere.cpp
	
StarsphereS5R3.o: $(DEPS) StarsphereS5R3.cpp
	$(CXX) -g ${CPPFLAGS} -c StarsphereS5R3.cpp

AbstractGraphicsEngine.o: AbstractGraphicsEngine.cpp
	$(CXX) -g ${CPPFLAGS} -c AbstractGraphicsEngine.cpp
	
GraphicsEngineFactory.o: GraphicsEngineFactory.cpp
	$(CXX) -g ${CPPFLAGS} -c GraphicsEngineFactory.cpp
	
WindowManager.o: Makefile WindowManager.cpp
	$(CXX) -g ${CPPFLAGS} -c WindowManager.cpp
	
BOINCClientAdapter.o: Makefile BOINCClientAdapter.cpp
	$(CXX) -g ${CPPFLAGS} -c BOINCClientAdapter.cpp
	
EinsteinS5R3Adapter.o: Makefile EinsteinS5R3Adapter.cpp
	$(CXX) -g ${CPPFLAGS} -c EinsteinS5R3Adapter.cpp

starlist.o: $(DEPS) starlist.C
	$(CXX) -g ${CPPFLAGS} -c starlist.C

snr_list.o: $(DEPS) snr_list.C
	$(CXX) -g ${CPPFLAGS} -c snr_list.C

pulsar_list.o: $(DEPS) pulsar_list.C
	$(CXX) -g ${CPPFLAGS} -c pulsar_list.C

# resource compiler
ResourceFactory.o: ${RESOURCESPEC}.o Resource.o ResourceFactory.cpp ResourceFactory.h
	$(CXX) -g ${CPPFLAGS} -c ResourceFactory.cpp -o ResourceFactory.o
	
Resource.o: Resource.cpp Resource.h
	$(CXX) -g ${CPPFLAGS} -c Resource.cpp -o Resource.o

${RESOURCESPEC}.o: ${RESOURCESPEC}.orc
	orc/orc ${RESOURCESPEC}.orc ${RESOURCESPEC}.cpp
	$(CXX) -g ${CPPFLAGS} -c ${RESOURCESPEC}.cpp -o ${RESOURCESPEC}.o


# tools
memcheck:
	valgrind --tool=memcheck --track-fds=yes --time-stamp=yes --log-file=${PWD}/memcheck.out.%p --leak-check=full ${PWD}/starsphere

callgrind:
	valgrind --tool=callgrind --track-fds=yes --time-stamp=yes ${PWD}/starsphere

clean:
	rm -f ${RESOURCESPEC}.cpp *.o starsphere

