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