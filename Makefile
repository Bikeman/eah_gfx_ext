# point this to where you installed BOINC (--prefix you gave to configure, defaults to $BOINC_PREFIX)
BOINC_PREFIX ?= /home/oliver/development/aei/boinc/bin
CXX ?= g++

# variables
LIBS = -L${BOINC_PREFIX}/lib $(shell sdl-config --libs) $(shell pkg-config --libs ftgl)
#LIBS = -L${BOINC_PREFIX}/lib -Wl,-Bstatic $(shell sdl-config --static-libs) -lfreetype -lz -lftgl -Wl,-Bdynamic -lGL -lGLU
CPPFLAGS = -I/usr/include $(shell sdl-config --cflags) $(shell pkg-config --cflags ftgl)
DEPS = Makefile starsphere.h 
OBJS = starlist.o snr_list.o user_text.o pulsar_list.o search_info.o starsphere.o
DEBUGFLAGSCPP = -DDEBUG -pg -ggdb -O0

# primary role based tagets
debug: starsphere
memcheck: starsphere
callgrind: starsphere
release:  clean starsphere

# target specific options
debug: CPPFLAGS += $(DEBUGFLAGSCPP)
memcheck: CPPFLAGS += $(DEBUGFLAGSCPP) -DDEBUG_VALGRIND
callgrind: CPPFLAGS += $(DEBUGFLAGSCPP) -DDEBUG_VALGRIND
release: CPPFLAGS += -O3 -Wall -Wno-switch-enum
release: LDFLAGS += -s

# file based targets
starsphere: $(DEPS) main.C $(OBJS)
	$(CXX) -g ${CPPFLAGS} ${LDFLAGS} main.C -o starsphere ${OBJS} ${LIBS}

starsphere.o: $(DEPS) starsphere.C
	$(CXX) -g ${CPPFLAGS} -I/usr/X11R6/include -c starsphere.C

starlist.o: $(DEPS) starlist.C
	$(CXX) -g ${CPPFLAGS} -c starlist.C

snr_list.o: $(DEPS) snr_list.C
	$(CXX) -g ${CPPFLAGS} -c snr_list.C

user_text.o: $(DEPS) user_text.C
	$(CXX) -g ${CPPFLAGS} -c user_text.C

pulsar_list.o: $(DEPS) pulsar_list.C
	$(CXX) -g ${CPPFLAGS} -c pulsar_list.C

search_info.o: $(DEPS) search_info.C
	$(CXX) -g ${CPPFLAGS} -c search_info.C
	
memcheck:
	valgrind --tool=memcheck --track-fds=yes --time-stamp=yes --log-file=${PWD}/memcheck.out.%p --leak-check=full ${PWD}/starsphere

callgrind:
	valgrind --tool=callgrind --track-fds=yes --time-stamp=yes ${PWD}/starsphere

clean:
	rm -f ${OBJS} starsphere

