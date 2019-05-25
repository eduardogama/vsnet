/**
 * @file MPDParser.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#ifndef MPDREQUESTHANDLER_H_
#define MPDREQUESTHANDLER_H_

#include <iostream>
#include <string>
#include <vector>

#include "parser/pugixml.hpp"


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

typedef struct MPDFile_t{
	string title; 
	string mimeType;
	int duration;
	vector<MPDSegment> segments;
} MPDFile;

class MPDRequestHandler {

    public:
    
        MPDRequestHandler();
        ~MPDRequestHandler();
    
        void ReadMPD(std::string path_mpd);

        vector<MPDSegment> &getSegments();

    protected:
        MPDFile mpd_handler;
};

#endif /* MPDREQUESTHANDLER_H_ */
