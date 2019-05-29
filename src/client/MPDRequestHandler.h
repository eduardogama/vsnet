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

typedef struct Representation_t {
	int id;
} Representation;

typedef struct AdaptationSet_t {
	int id;
	int group;
	bool bitstreamSwitching;
	int maxWidth;
	int maxHeight;
	int maxFrameRate;
	std::string par;
	vector<MPDSegment> segments;
	vector<Representation> Rrepresentations;
} AdaptationSet;

typedef struct MPDFile_t{
	string title; 
	string mimeType;
	int duration;
/*	vector<AdaptationSet> adaptationset;*/
	vector<MPDSegment> segments;
} MPDFile;


class MPDRequestHandler {

    public:
    
        MPDRequestHandler();
        ~MPDRequestHandler();

        void printSegments();

        void ReadMPD(std::string path_mpd);

        vector<MPDSegment> &getSegments();
        MPDSegment &getSegment(int value);

    protected:
        MPDFile mpd_handler;
};

#endif /* MPDREQUESTHANDLER_H_ */
