/**
 * @file MPDParser.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#ifndef MPDPARSER_H_
#define MPDPARSER_H_

#include <iostream>
#include <string>
#include <vector>

#include <stdlib.h>

#include "pugixml.hpp"


using namespace std;


typedef struct MPDSegment_t{
    string id;
    string mimeType;
    string codecs;
	int frameRate;
	int width;
	int height;
	int bandwidth;
} MPDSegment;

typedef struct MPDRequestHandler_t{
	string title; 
	string mimeType;
	int duration;
	vector<MPDSegment> segments;
} MPDRequestHandler;

class MPDParser {

    public:
    
        MPDParser();
        ~MPDParser();
    
        MPDRequestHandler ReadMPD(std::string path_mpd);

    protected:
        MPDRequestHandler mpd_handler;
};

#endif /* MPDPARSER_H_ */   
