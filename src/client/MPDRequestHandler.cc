/**
 * @file MPDParser.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#include "MPDRequestHandler.h"


MPDRequestHandler::MPDRequestHandler()
{}

MPDRequestHandler::~MPDRequestHandler()
{}

void MPDRequestHandler::ReadMPD(std::string path_mpd)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path_mpd.c_str());
    
    if (!result){
    	std::cout << "Path mpd file Error !" << std::endl;
        return;
    }
    
    mpd_handler.title    = doc.child("MPD").child("ProgramInformation").child_value("Title");
    mpd_handler.duration = doc.child("MPD").child("Period").child("AdaptationSet").child("SegmentTemplate").attribute("duration").as_int();
    
    
//  std::cout << mpd_handler.title    << std::endl;
//	std::cout << mpd_handler.duration << std::endl;
    
    for (pugi::xml_node tool = doc.child("MPD").child("Period").child("AdaptationSet").first_child(); tool; tool = tool.next_sibling())
    {
        MPDSegment segment;

        segment.id        = tool.attribute("id").value();
        segment.mimeType  = tool.attribute("mimeType").value();
        segment.codecs    = tool.attribute("codecs").value();
        segment.frameRate = tool.attribute("frameRate").as_int();
        segment.width     = tool.attribute("width").as_int();
        segment.height    = tool.attribute("height").as_int();
        segment.bandwidth = tool.attribute("bandwidth").as_int();

        mpd_handler.segments.push_back(segment);
    }
}

vector<MPDSegment> &MPDRequestHandler::getSegments()
{
    return (mpd_handler.segments);
}
