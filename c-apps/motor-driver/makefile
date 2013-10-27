# Makefile
# Author: Arvind Pereira

# Modify the includes, sources, headers, executable and
# other flags accordingly!
INCLUDES = -I ./ -I/usr/local/include -I../spi-mover
LIBS = -L/usr/lib/ -lm -lpthread

CC = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = -Wall -c

SOURCES = main.cpp \
		dspin_driver.cpp 

SUBOBJS = ../spi-mover/spi_mover.o

HEADERS = dspin_driver.h \
		string_cb.h
#      File2.h

OBJ = $(SOURCES:.cpp=.o)

EXE = test

all: test

$(EXE):    $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(SUBOBJS) $(LFLAGS) $(LIBS)    

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	
spi-mover.o:
	cp ../spi-mover/spi_mover.o

#tar:
#    tar -czf $(EXE).tar.gz $(SOURCES) $(HEADERS) Makefile

clean:
	rm -f $(OBJ)
	rm -f $(EXE)