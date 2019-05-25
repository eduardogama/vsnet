/**
 * @file DashClient.cc
 * @author Eduardo S Gama
 */

#include "DashClient.h"


Define_Module(DashClient);

DashClient::DashClient()
{
}

DashClient::~DashClient()
{
}

void DashClient::initialize(int stage)
{
	TcpAppBase::initialize(stage);

	if (INITSTAGE_APPLICATION_LAYER != 3) return;

	mpd.ReadMPD("sample.mpd");


	// read Adaptive Video (AV) parameters
    const char *str = par("video_packet_size_per_second").stringValue();
    video_packet_size_per_second = cStringTokenizer(str).asIntVector(); //vector <1000,1500,2000,4000,8000,12000> in kbits

    video_buffer_max_length = par("video_buffer_max_length"); // 10s
    video_duration          = par("video_duration"); // 30s
    manifest_size           = par("manifest_size"); // 100000

    numRequestsToSend = video_duration;

    video_buffer_min_rebuffering = 3; // if video_buffer < video_buffer_min_rebuffering then a rebuffering event occurs
    video_buffer                 = 0;
    video_playback_pointer       = 0;
    video_current_quality_index  = 0;  // start with min quality
    video_is_playing             = false;

	WATCH(video_buffer);
    WATCH(video_playback_pointer);
    WATCH(numRequestsToSend);


    startTime = par("startTime"); // 1s
    stopTime  = par("stopTime"); // 0 means infinity

    if (stopTime != 0 && stopTime <= startTime)
        error("Invalid startTime/stopTime parameters");


//	bufferMapExchangePeriod = par("bufferMapExchangePeriod");


//	chunkSize = par ("chunkSize");
//	
//	numOfBFrame = par("numOfBFrame");
//	for (int i=0;i<10;i++)
//	{
//		bufferSize[i] =0;
//		videolenght[i]=0;
//		if (isVideoServer==true)
//			LV->watchFilm[i]=true;
//	}
    
    //LV->
//    rateControl = par("rateControl");
//    maxFrameRequestPerBufferMapExchangePeriod=par("maxFrameRequestPerBufferMapExchangePeriod");
//    numberOfFrameRequested=0;
//    measuringTime = par("measuringTime");
//    receiverSideSchedulingNumber = par("receiverSideSchedulingNumber");
//    senderSideSchedulingNumber = par("senderSideSchedulingNumber");
//    averageChunkLength = par("averageChunkLength");
//	
//	//initialize self messages
//    structureImprovementTimer=new cMessage("structureImprovementTimer");
//    improvmentStructure=par("improvmentStructure");
//    improvementPeriod=par("improvementPeriod");
//    bufferMapTimer = new cMessage("bufferMapTimer");
//	requestChunkTimer = new cMessage("requestChunkTimer");
//	playingTimer = new cMessage("playingTimer");
//	sendFrameTimer = new cMessage("sendFrameTimer");

    getParentModule()->getParentModule()->setDisplayString("i=device/wifilaptop_vs;i2=block/circle_s");
}

void DashClient::decodePacket(Packet *vp)
{

}

void DashClient::ReadMPD()
{
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("sample.mpd");
    
    if (!result){
    	std::cout << "ERROR" << std::endl;
        return;
    }
    
    dashplayback.title    = doc.child("MPD").child("ProgramInformation").child_value("Title");
    dashplayback.duration = doc.child("MPD").child("Period").child("AdaptationSet").child("SegmentTemplate").attribute("duration").as_int();
    
    
    std::cout << result.description()  << std::endl;
    std::cout << dashplayback.title    << std::endl;
	std::cout << dashplayback.duration << std::endl;	
}
