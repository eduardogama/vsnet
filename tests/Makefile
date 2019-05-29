CC=g++
CFLAGS=
INCLUDE_PATH = -I. \
			   -I../src \
			   -I../src/client \
			   -I../src/parser \

SRC = main.cpp 
OBJ := $(SRC:%.cpp=/%.o)

SRC_PARSER = pugixml.cc
OBJ_PARSER := $(SRC_PARSER:%.cpp=/%.o)

SRC_CLIENT = MPDRequestHandler.cc 
OBJ_CLIENT := $(SRC_CLIENT:%.cpp=/%.o)
		   
APP = main

all: $(APP)


$(APP): 
	@echo "Linking Executable $(APP)"
	$(CC) -o $(APP) $? $(INCLUDE_PATH) $(LIB_PATH) $(CFLAGS)


%.o: $(SRC) $(SRC_PARSER) $(SRC_CLIENT)
	$(CC) -c $(CFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $< -o $@

help:
	@echo "src: $(SRC)"
	@echo "src_parser: $(SRC_PARSER)"
	@echo "src_client: $(SRC_CLIENT)"

clean:
	rm -rf *o main
	
