BASE_LIBS=-lboost_program_options
BASE_CFLAGS=-c -g -O2 -Wall -std=c++11

PROGRAMNAME=smfltest4

O_FILES=${PROGRAMNAME}.o

BASE_LIBS+=-lsfml-graphics -lsfml-window -lsfml-system  -lphysfs
O_FILES+= TextureHolder.o SagoSprite.o

total: ${PROGRAMNAME}

clean: 
	rm -f */*.o *.o *.P */*.P ${PROGRAMNAME}

${PROGRAMNAME}: $(O_FILES)
	g++ -O -o ${PROGRAMNAME} $(O_FILES) $(BASE_LIBS)

%.o : %.cpp
	g++ -MD ${BASE_CFLAGS} -o $@ $<
	@cp $*.d $*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
             rm -f $*.d

-include *.P
