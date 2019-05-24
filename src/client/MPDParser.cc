/**
 * @file MPDParser.h
 * @author Eduardo S Gama, State University of Campinas (Unicamp)
 */

#include "MPDParser.h"


MPDParser::MPDParser()
{}

MPDParser::~MPDParser()
{}

MPDRequestHandler MPDParser::ReadMPD(std::string path_mpd)
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample.mpd");
    
    if (!result){
    	std::cout << "ERROR" << std::endl;
        return mpd_handler;
    }
    
    mpd_handler.title    = doc.child("MPD").child("ProgramInformation").child_value("Title");
    mpd_handler.duration = doc.child("MPD").child("Period").child("AdaptationSet").child("SegmentTemplate").attribute("duration").as_int();
    
    
    std::cout << mpd_handler.title    << std::endl;
	std::cout << mpd_handler.duration << std::endl;	
    
    
    for (pugi::xml_node tool = doc.child("MPD").child("Period").child("AdaptationSet").first_child(); tool; tool = tool.next_sibling())
    {
        MPDSegment segment;
     
        cout << "===================================================" << endl;
        
        segment.id        = tool.attribute("id").value();
        segment.mimeType  = tool.attribute("mimeType").value();
        segment.codecs    = tool.attribute("codecs").value();
        segment.frameRate = tool.attribute("frameRate").as_int();
        segment.width     = tool.attribute("width").as_int();
        segment.height    = tool.attribute("height").as_int();
        segment.bandwidth = tool.attribute("bandwidth").as_int();

    //        cout  << "Id="        << segment.id         << endl
    //              << "mimeType="  << segment.mimeType   << endl
    //              << "codecs="    << segment.codecs     << endl
    //              << "frameRate=" << segment.frameRate  << endl
    //              << "width="     << segment.width      << endl
    //              << "height="    << segment.height     << endl
    //              << "bandwidth=" << segment.bandwidth  << endl; 
              
        mpd_handler.segments.push_back(segment);
    }
    
    for (std::vector<MPDSegment>::iterator it = mpd_handler.segments.begin() ; it != mpd_handler.segments.end(); ++it){
        cout  << "Id="        << (*it).id         << endl
              << "mimeType="  << (*it).mimeType   << endl
              << "codecs="    << (*it).codecs     << endl
              << "frameRate=" << (*it).frameRate  << endl
              << "width="     << (*it).width      << endl
              << "height="    << (*it).height     << endl
              << "bandwidth=" << (*it).bandwidth  << endl;
        cout << "===================================================" << endl;
    }
    
    return mpd_handler;
}


