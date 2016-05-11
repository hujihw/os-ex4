# define compiler and flags
CC=g++
CXXFLAGS=--std=c++11 -Wall
FUSE=`pkg-config fuse --cflags --libs`

# executable variables
EXECSOURCES=CachingFileSystem.cpp
EXECHEADERS=
EXECOBJECTS=$(EXECSOURCES:.cpp=.o)

# target variables
EXECUTABLE=CachingFileSystem
VALGRINDEXEC=vCachingFileSystem
TAR=ex4.tar


.PHONY: all tar valgrind clean


all: $(EXECUTABLE)

# create the executable
$(EXECUTABLE): $(EXECSOURCES) $(EXECHEADERS)
	$(CC) $(CXXFLAGS) $(EXECSOURCES) $(FUSE) -o $(EXECUTABLE)

# compile for Valgrind debugging
valgrind: $(EXECSOURCES) $(EXECHEADERS)
	$(CC) $(CXXFLAGS) -g $(EXECSOURCES) $(FUSE) -o $(VALGRINDEXEC)

# create a .tar file for submission
tar: $(EXECSOURCES) $(EXECHEADERS) Makefile README
	tar cvf $(TAR) $^

# clean all files created by this Makefile
clean:
	rm -f *.o *.tar $(VALGRINDEXEC) $(EXECUTABLE)
