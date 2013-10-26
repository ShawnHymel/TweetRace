# Makefile
# Author: Arvind Pereira

# Modify the includes, sources, headers, executable and
# other flags accordingly!
#
# Borrowed & modded by Byron Jacquot
#
INCLUDES = -I ./ -I/usr/local/include
LIBS = -L/usr/lib/ -lm

CC = g++
DEBUG = -g
LFLAGS = -Wall $(DEBUG)
CFLAGS = -Wall -c

SOURCES = test.cpp \
		spi_mover.cpp \
		shim.cpp
#      File3.cpp

HEADERS = spi_mover.h \
		shim.h
#      File2.h

OBJ = $(SOURCES:.cpp=.o)

EXE = prog

all: prog

$(EXE):    $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LFLAGS) $(LIBS)    

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

#tar:
#    tar -czf $(EXE).tar.gz $(SOURCES) $(HEADERS) Makefile

clean:
	rm -f $(OBJ)
	rm -f $(EXE)
