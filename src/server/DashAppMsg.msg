

import inet.common.INETDefs;
import inet.common.Units;
import inet.common.packet.chunk.Chunk;

namespace inet;

message DashAppMsg extends FieldsChunk
{
    B expectedReplyLength;   // in bytes
    double replyDelay;       // reply after this many seconds
    bool serverClose;        // with TCP: if true, server should close the
                             // connection after sending the reply
	int StartByte;
    int EndByte;
    string redirectAddress;
}