# Makefile
# Author: Arvind Pereira

# Modify the includes, sources, headers, executable and
# other flags accordingly!
#
# Byron Jacquot: this isn't making a chared library properly...probably needs to invoke AR?
# or at least name stuff as "lib*"??
#
# For the moment, users are statically linking to to .o
#
INCLUDES = -I ./ -I/usr/local/include
#LIBS = -L/usr/lib/ -lm

CC = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = -Wall -c

SOURCES = spi_mover.cpp \
	test.cpp
#      File3.cpp

HEADERS = spi_mover.h 
#      File2.h

OBJ = $(SOURCES:.cpp=.o)

#OUTLIB = spi_mover.so
EXE = test

all: test

$(EXE):    $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(SUBOBJS) $(LFLAGS) $(LIBS)    

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

#tar:
#    tar -czf $(EXE).tar.gz $(SOURCES) $(HEADERS) Makefile

clean:
	rm -f $(OBJ)
	rm -f $(EXE)