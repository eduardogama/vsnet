#include "pugixml.hpp"
#include <iostream>

int main()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample.mpd");
    
    if (!result){
    	std::cout << "ERROR" << std::endl;
        return -1;
    }
    
    std::cout << result.description() << std::endl;
    std::cout << doc.child("MPD").child("ProgramInformation").child_value("Title") << std::endl;
	std::cout << doc.child("MPD").child("Period").attribute("duration").value() << std::endl;
    
//    for (pugi::xml_node tool: doc.child("Profile").child("Tools").children("Tool"))
//    {
//        int timeout = tool.attribute("Timeout").as_int();
//        
//        if (timeout > 0)
//            std::cout << "Tool " << tool.attribute("Filename").value() << " has timeout " << timeout << "\n";
//    }

}


