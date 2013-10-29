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

SOURCES = hoss_test.cpp \
		hoss_system.cpp 

#SUBOBJS = ../spi-mover/spi_mover.o

HEADERS = hoss_system.h
#      File2.h

OBJ = $(SOURCES:.cpp=.o)

EXE = hosstest

all: hosstest

$(EXE):    $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(SUBOBJS) $(LFLAGS) $(LIBS)    

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@
	

#tar:
#    tar -czf $(EXE).tar.gz $(SOURCES) $(HEADERS) Makefile

clean:
	rm -f $(OBJ)
	rm -f $(EXE)