/**
 * @file MPDParser.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#include "MPDRequestHandler.h"

#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>


MPDRequestHandler::MPDRequestHandler()
{}

MPDRequestHandler::~MPDRequestHandler()
{}


void MPDRequestHandler::ReadMPD(std::string path_mpd)
{
//    ReadSingleFile(std::string path_mpd);
    ReadMultipleFiles(path_mpd);
}

void MPDRequestHandler::ReadSingleFile(std::string path_mpd)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path_mpd.c_str());
    
    if (!result){
    	std::cout << "Path mpd file Error !" << std::endl;
        return;
    }
    
    mpdhandler.title = doc.child("MPD").child("ProgramInformation").child_value("Title");


    this->mediaPresentationDuration = ParserTime(doc.child("MPD").attribute("mediaPresentationDuration").value());
    this->minBufferTime             = ParserTime(doc.child("MPD").attribute("minBufferTime").value());

//	std::string duration = doc.child("MPD").attribute("mediaPresentationDuration");
    std::cout << this->mediaPresentationDuration << std::endl;
    std::cout << this->minBufferTime << std::endl;

    for (pugi::xml_node tool = doc.child("MPD").child("Period").child("AdaptationSet").first_child(); tool; tool = tool.next_sibling())
    {
        if(std::string(tool.name()) != "Representation")
            continue;

        MPDSegment *segment = new MPDSegment();

//        segment->id        = tool.attribute("id").value();
//        segment->mimeType  = tool.attribute("mimeType").value();
//        segment->codecs    = tool.attribute("codecs").value();
//        represen->frameRate = tool.attribute("frameRate").as_int();
//        segment->width     = tool.attribute("width").as_int();
//        segment->height    = tool.attribute("height").as_int();
//        segment->bandwidth = tool.attribute("bandwidth").as_int();

        getSegments().push_back(*segment);
    }
}

void MPDRequestHandler::ReadMultipleFiles(std::string mpd_path)
{
    struct dirent *entry;

    DIR *dir = opendir(mpd_path.c_str());

    if (dir == NULL) return;

    quality.push_back(std::string("360"));
    quality.push_back(std::string("720"));
    quality.push_back(std::string("1080"));

    this->segIndex = 0;

    while ((entry = readdir(dir)) != NULL) {
        if(std::string(entry->d_name) == "." || std::string(entry->d_name) == "..")
            continue;

        std::string g_path = mpd_path + "/" + entry->d_name + "/stream.mpd";

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(g_path.c_str());

        if (!result) {
            std::cout << "Path mpd file Error " << g_path <<  "!" << std::endl;
            return;
        }

        this->mediaPresentationDuration = ParserTime(doc.child("MPD").attribute("mediaPresentationDuration").value());
        this->minBufferTime             = ParserTime(doc.child("MPD").attribute("minBufferTime").value());

        Representation rp;

        pugi::xml_node seg_xml_node = doc.child("MPD").child("Period").child("AdaptationSet")
                .child("Representation").child("SegmentList").first_child();

        for (pugi::xml_node tool = seg_xml_node ; tool; tool = tool.next_sibling()) {
            if( (std::string(tool.name()) == "SegmentURL")) {

                std::string mde = tool.attribute("media").value();
                std::string mdeRange = tool.attribute("mediaRange").value();

                vector<string> range{Split(mdeRange,'-')};

                MPDSegment segment;

                segment.media      = tool.attribute("media").value();
                segment.mediaRange = std::stoi(range[1])-std::stoi(range[0]);

                rp.segments.push_back(segment);
            }
        }

        std::pair<std::string, Representation> p(std::string(entry->d_name), rp);
        this->representation.insert(p);

        this->numSegments = this->representation[entry->d_name].segments.size();
    }

    closedir(dir);
}

const vector<string> MPDRequestHandler::Split(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}

simtime_t MPDRequestHandler::ParserTime(std::string str)
{
    simtime_t tmp;

    char num[10];

    for (int str_i=2,num_i=0; str_i < str.size(); str_i++) {

        for(num_i=0; str[str_i] != 'M' && str[str_i] != 'S'; num_i++, str_i++) {
            num[num_i] = str[str_i];
        }
        num[num_i] = '\0';

        if(str[str_i] == 'M'){
//            std::cout << "Parser M=" << num << std::endl;
            tmp += atof(num) * 60;
        }else if(str[str_i] == 'S'){
//            std::cout << "Parser S=" << num << std::endl; //Print two times. see it later
            tmp += atof(num);
        }

    }

    return tmp;
}

int MPDRequestHandler::NumSegments()
{
    return this->numSegments;
}

vector<MPDSegment> &MPDRequestHandler::getSegments()
{
    return (mpdhandler.segments);
}

MPDSegment &MPDRequestHandler::getHighRepresentation()
{
    return mpdhandler.segments[mpdhandler.segments.size()-1];
}

MPDSegment &MPDRequestHandler::getLowRepresentation()
{
    if(mpdhandler.segments.size() != 0){
        return mpdhandler.segments[0];
    } else {
        return (representation[quality[0]]).segments[0];
    }
}

Segment *MPDRequestHandler::LowRepresentation()
{
    Segment *seg = new Segment();

    simtime_t time = simTime();
    int size = this->representation[quality[0]].segments[this->segIndex].mediaRange;
    std::string q = quality[0];

    seg->setStartTime(time);
    seg->setSegmentSize(size);
    seg->setQuality(q);
    seg->setSegmentNumber(this->segIndex);

    this->segIndex += 1;

    return seg;
}

MPDSegment &MPDRequestHandler::getSegment(int value)
{
    return mpdhandler.segments[value];
}

simtime_t MPDRequestHandler::getMediaPresentationDuration()
{
    return this->mediaPresentationDuration;
}

simtime_t MPDRequestHandler::getMaxSegmentDuration()
{
    return this->maxSegmentDuration;
}

simtime_t MPDRequestHandler::getMinBufferTime()
{
    return this->minBufferTime;
}

void MPDRequestHandler::printSegments()
{
    std::cout << "Segments Size="<< getSegments().size() << std::endl;

    for (std::vector<MPDSegment>::iterator it = getSegments().begin() ; it != getSegments().end(); ++it){
        std::cout << "===================================================" << std::endl;
        std::cout << "Media="        << (*it).media      << endl
                  << "mimeType="     << (*it).mediaRange << endl;
//                  << "codecs="       << (*it).codecs     << endl
//                  << "frameRate=" << (*it).frameRate  << endl
//                  << "width="     << (*it).width      << endl
//                  << "height="    << (*it).height     << endl
//                  << "bandwidth=" << (*it).bandwidth  << endl;
        std::cout << "===================================================" << std::endl;
    }
}
