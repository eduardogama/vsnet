/**
 * @file MPDParser.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#ifndef MPDREQUESTHANDLER_H_
#define MPDREQUESTHANDLER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <omnetpp/simtime_t.h>

#include <stdlib.h>

#include "client/Segment.h"
#include "parser/pugixml.hpp"


using namespace std;
//using namespace inet;
using namespace omnetpp;


typedef struct MPDSegment_t{
//    string id;
//    string mimeType;
//    string codecs;
    string media;
    int mediaRange;
//	int frameRate;
//	int width;
//	int height;
//	int bandwidth;
} MPDSegment;

typedef struct Representation_t {
	int id;
	std::string bandwidth;
	int frameRate;
    int width;
    int height;
    vector<MPDSegment> segments;
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
        static MPDRequestHandler* getInstance();

        ~MPDRequestHandler();

        void printSegments();

        void ReadMPD(std::string path_mpd);

        void ReadSingleFile(std::string path_mpd);

        void ReadMultipleFiles(std::string path_mpd);

        vector<MPDSegment> &getSegments();

        MPDSegment &getHighRepresentation();

        MPDSegment &getLowRepresentation();

        MPDSegment &getSegment(int value);

        Segment *LowRepresentation();

        Segment *HighRepresentation(int segIndex);

        simtime_t getMediaPresentationDuration();

        simtime_t getMaxSegmentDuration();

        simtime_t getMinBufferTime();

        const vector<string> Split(const string& s, const char& c);

        int NumSegments();

        vector<std::string>& getQuality();

        void setQuality(const vector<std::string>& quality);

        map<std::string, Representation>& getRepresentation();

        void setRepresentation(map<std::string, Representation>& representation);

    private:

        MPDRequestHandler();

        simtime_t ParserTime(std::string str);

    protected:
        static MPDRequestHandler* instance;

        simtime_t mediaPresentationDuration;
        simtime_t maxSegmentDuration;
        simtime_t minBufferTime;

        map<std::string, Representation> representation;

        vector<std::string> quality;
        int segIndex;
        int numSegments;

        MPDFile mpdhandler;
};

#endif /* MPDREQUESTHANDLER_H_ */
