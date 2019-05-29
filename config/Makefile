CC=g++
CFLAGS= -ldash -lxml2# -W -Wall -ansi -pedantic -Wno-unknown-pragmas
INCLUDE_PATH = -I. \
			   -I/home/eduardo/github/libdash/libdash/libdash/include \
			   -I/home/eduardo/github/libdash/libdash/libdash_networkpart_test \
			   -I/home/eduardo/github/libdash/libdash/libdash/source \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/manager \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/metrics \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/helpers \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/mpd \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/portable \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/xml \
			   -I/home/eduardo/github/libdash/libdash/libdash/source/network
			   

LIB_PATH = -L/home/eduardo/github/libdash/libdash/build/bin

#SRCDIR = .			   

#SRC := $(shell find $(SRCDIR) -name "*.cpp")
#OBJ := $(SRC:%.cpp=$(OBJDIR)/%.o)

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eduardo/github/libdash/libdash

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eduardo/github/libdash/libdash/build

#=============================================================================

SRC = $(wildcard *.cpp)
OBJ_SRC = $(notdir $(SRC:.cpp=.o))

SRC_LIBDASH	= $(wildcard /home/eduardo/github/libdash/libdash/libdash/source/*.cpp) 
OBJ_SRC_LIBDASH = $(notdir $(SRC_LIBDASH:.cpp=.o))

SRC_LIBDASH	= $(wildcard /home/eduardo/github/libdash/libdash/libdash/source/*.cpp) 
OBJ_SRC_LIBDASH = $(notdir $(SRC_LIBDASH:.cpp=.o))

APP = main



all: $(APP)


$(APP): $(OBJ_SRC)
	@echo "Linking Executable $(APP)"
	$(CC) -o $(APP) $? $(INCLUDE_PATH) $(LIB_PATH) $(CFLAGS)


#$(OBJDIR)/%.o: %.cpp 
#	@echo "Compiling Source $< Into Object $@ $(@D)"
#	@mkdir -p '$(@D)'
#	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $< -o $@


%.o: %.cpp
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $< -o $@

help:
	@echo "src: $(SRC)"
	@echo "obj: $(OBJ_SRC)"
	@echo "src: $(SRC_LIBDASH)"
	@echo "obj: $(OBJ_SRC_LIBDASH)"

clean:
	rm -rf *o main
	
