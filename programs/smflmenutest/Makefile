BASE_LIBS=-lboost_program_options
BASE_CFLAGS=-c -g -O2 -Wall -std=c++11

PROGRAMNAME=smflmenutest

O_FILES=${PROGRAMNAME}.o

BASE_LIBS+=-lsfml-graphics -lsfml-window -lsfml-system  -lphysfs
O_FILES+= SagoDataHolder.o SagoMenu.o SagoCommandQueue.o

total: ${PROGRAMNAME}

clean: 
	rm -f */*.o *.o *.P */*.P ${PROGRAMNAME}

${PROGRAMNAME}: $(O_FILES)
	$(CXX) -O -o ${PROGRAMNAME} $(O_FILES) $(BASE_LIBS)

%.o : %.cpp
	$(CXX) -MD ${BASE_CFLAGS} -o $@ $<
	@cp $*.d $*.P; \
             sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
                 -e '/^$$/ d' -e 's/$$/ :/' < $*.d >> $*.P; \
             rm -f $*.d

-include *.P
