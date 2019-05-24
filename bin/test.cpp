//#include "../src/client/DashClient.h"

#include <pthread.h>
#include <SDL2/SDL.h>


#include "MPDParser.h"

int main()
{
    
    int i;
    
    SDL_Event event;
    
    pthread_t rtspthread;
    pthread_t ctrlthread;
    pthread_t watchdog;
    
	std::string savefile_keyts;
    
    
    MPDParser mpd;
    
    mpd.ReadMPD("sample.mpd");
    
//	DashClient client;
//	client.initialize(3);


	return 0;
}
