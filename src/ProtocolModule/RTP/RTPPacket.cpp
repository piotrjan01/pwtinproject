/*
 * RTPPacket.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include "RTPPacket.h"

RTPPacket::RTPPacket() {
	header = new RTPHeader();
	data = NULL;
}

RTPPacket::RTPPacket(char* packet, int _dataSize) {
	if ((unsigned) _dataSize < RTPHeader::SIZE_IN_BYTES) {
		throw DeserializationException();
	}
	header = new RTPHeader(packet);
	dataSize = _dataSize - RTPHeader::SIZE_IN_BYTES;
	memcpy(data, packet + RTPHeader::SIZE_IN_BYTES, dataSize);
}

RTPPacket::RTPPacket(RTPHeader& hdr, char* packetData, int _dataSize) {
	*header = hdr;
	dataSize = _dataSize;
	memcpy(data, packetData, dataSize);
}

RTPPacket::~RTPPacket() {
	// TODO Auto-generated destructor stub
}

string RTPPacket::toStream() {
	string result = header->toStream();
	result += data;
	return result;
}

