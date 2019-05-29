#!/bin/bash

g++ -o main main.cpp ../src/client/MPDRequestHandler.cc ../src/parser/pugixml.cc  -I../src
