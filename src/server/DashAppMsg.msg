

import inet.common.INETDefs;
import inet.common.Units;
import inet.common.packet.chunk.Chunk;

namespace inet;

class DashAppMsg extends FieldsChunk
{
    B expectedReplyLength;   // in bytes
    B contentLength;
    double replyDelay;       // reply after this many seconds
    bool serverClose;        // with TCP: if true, server should close the
                             // connection after sending the reply
	int StartByte;
    int EndByte;
    string redirectAddress;
    
    int num_segment;
    int bitrate;
    string resolution;
}