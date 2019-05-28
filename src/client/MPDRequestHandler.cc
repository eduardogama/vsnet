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
    
    std::cout << mpd_handler.title    << std::endl;
	std::cout << mpd_handler.duration << std::endl;

    for (pugi::xml_node tool = doc.child("MPD").child("Period").child("AdaptationSet").first_child(); tool; tool = tool.next_sibling())
    {
        if(std::string(tool.name()) != "Representation")
            continue;

        MPDSegment *segment = new MPDSegment();

        segment->id        = tool.attribute("id").value();
        segment->mimeType  = tool.attribute("mimeType").value();
        segment->codecs    = tool.attribute("codecs").value();
        segment->frameRate = tool.attribute("frameRate").as_int();
        segment->width     = tool.attribute("width").as_int();
        segment->height    = tool.attribute("height").as_int();
        segment->bandwidth = tool.attribute("bandwidth").as_int();

        getSegments().push_back(*segment);
    }
}

void MPDRequestHandler::printSegments()
{
    std::cout << "Segments Size="<< getSegments().size() << std::endl;

    for (std::vector<MPDSegment>::iterator it = getSegments().begin() ; it != getSegments().end(); ++it){
        std::cout << "===================================================" << std::endl;
        std::cout << "Id="        << (*it).id         << endl
                  << "mimeType="  << (*it).mimeType   << endl
                  << "codecs="    << (*it).codecs     << endl
                  << "frameRate=" << (*it).frameRate  << endl
                  << "width="     << (*it).width      << endl
                  << "height="    << (*it).height     << endl
                  << "bandwidth=" << (*it).bandwidth  << endl;
        std::cout << "===================================================" << std::endl;
    }
}

vector<MPDSegment> &MPDRequestHandler::getSegments()
{
    return (mpd_handler.segments);
}

MPDSegment &MPDRequestHandler::getSegment(int value)
{
    return mpd_handler.segments[value];
}
