//#include "../src/client/DashClient.h"

#include <pthread.h>
#include <SDL2/SDL.h>


#include "../src/client/MPDRequestHandler.h"

int main()
{
    MPDRequestHandler mpd;
    
    
    mpd.ReadMPD("../input/multi-codec.mpd");
    
    
    
    
	return 0;
}
