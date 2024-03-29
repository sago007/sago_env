CC=$(CROSS)gcc
CXX=$(CROSS)g++
LD=$(CROSS)ld
AR=$(CROSS)ar
PKG_CONFIG=$(CROSS)pkg-config

-include sago/Makefile.sago

BASE_LIBS=$(shell $(CROSS)sdl2-config --libs) -lSDL2_ttf $(shell $(CROSS)pkg-config SDL2_image SDL2_mixer --libs) $(SAGO_BASE_LIBS)
BASE_CFLAGS=-c -g -O2 -Wall -std=c++11 $(shell $(CROSS)sdl2-config --cflags)

PROGRAMNAME=sdl2test4

O_FILES=${PROGRAMNAME}.o Libs/NFont.o Libs/SDL_FontCache.o ${SAGO_O_FILES}

total: ${PROGRAMNAME}

clean: 
	rm -f */*.o *.o *.P */*.P ${PROGRAMNAME}

${PROGRAMNAME}: $(O_FILES)
	$(CXX) -O -o ${PROGRAMNAME} $(O_FILES) $(BASE_LIBS)

%.o : %.cpp
	$(CXX) -MD ${BASE_CFLAGS} -o $@ $<
	@mkdir -p `dirname .$(CROSS)deps/$*.P` && cp $*.d .$(CROSS)deps/$*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> .$(CROSS)deps/$*.P; \
             rm -f $*.d

-include .$(CROSS)deps/*.P

SOURCE_FILES = $(O_FILES:.o=.cpp)

format:
	astyle -t -j -y -c -k1 -z2 -A2 --pad-header $(SOURCE_FILES) 
