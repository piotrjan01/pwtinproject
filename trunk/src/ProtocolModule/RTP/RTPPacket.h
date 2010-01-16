/*
 * RTPPacket.h
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#ifndef RTPPACKET_H_
#define RTPPACKET_H_

#include "RTPHeader.h"
#include "DeserializationException.h"

#include <string>
#include <cstring>

using namespace std;

class RTPPacket {
	static const int DEFAULT_DELAY = 0;
public:
	RTPPacket();
	RTPPacket(const RTPPacket& another);

	RTPPacket(char* data, int dataSize);
	RTPPacket(const RTPHeader& hdr, char* data, int dataSize);
	virtual ~RTPPacket();

	ostream& toStream(ostream& os);
	string toString();

	RTPPacket& operator=(const RTPPacket& another);

	long delay;	// delay that should be used for sending the packet

	RTPHeader* header;

	char* data;
	unsigned dataSize;
};

#endif /* RTPPACKET_H_ */
