# point this to where you installed BOINC (--prefix you gave to configure, defaults to $BOINC_PREFIX)
BOINC_PREFIX ?= /home/oliver/development/aei/boinc/bin

LIBS= -L${BOINC_PREFIX}/lib -lSDL -lGL -lGLU
CPPFLAGS=-I/usr/include
DEPS=Makefile starsphere.h 
OBJS=starlist.o snr_list.o user_text.o pulsar_list.o search_info.o starsphere.o

CXX ?= g++

starsphere: $(DEPS) main.C $(OBJS)
	$(CXX) -g ${CPPFLAGS} main.C -o starsphere ${OBJS} ${LIBS}

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

clean:
	rm ${OBJS} starsphere

