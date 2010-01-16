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
public:
	RTPPacket();
	RTPPacket(char* data, int dataSize);
	RTPPacket(RTPHeader& hdr, char* data, int dataSize);
	virtual ~RTPPacket();

	string toStream();

	long delay;
	RTPHeader* header;

private:
	char* data;
	unsigned dataSize;
};

#endif /* RTPPACKET_H_ */
