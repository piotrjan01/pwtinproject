/*
 * RTPPacket.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include "RTPPacket.h"

RTPPacket::RTPPacket() :
	delay(DEFAULT_DELAY) {
	header = new RTPHeader();
	data = NULL;
	dataSize = 0;
}

RTPPacket::RTPPacket(const RTPPacket& another) {
	delay = another.delay;
	header = new RTPHeader(*another.header);
	dataSize = another.dataSize;
	data = new char[another.dataSize];
	memcpy(data, another.data, another.dataSize);
}

RTPPacket::RTPPacket(char* packet, int _dataSize) {
	if ((unsigned) _dataSize < RTPHeader::SIZE_IN_BYTES) {
		throw DeserializationException();
	}
	header = new RTPHeader(packet);
	dataSize = _dataSize - RTPHeader::SIZE_IN_BYTES;
	data = new char[dataSize];
	memcpy(data, packet + RTPHeader::SIZE_IN_BYTES, dataSize);
}

RTPPacket& RTPPacket::operator=(const RTPPacket& another) {
	if (this == &another) {
		return *this;
	}
	delay = another.delay;
	*header = *another.header;
	dataSize = another.dataSize;
	memcpy(data, another.data, another.dataSize);
	return *this;
}

RTPPacket::RTPPacket(const RTPHeader& hdr, char* packetData, int _dataSize) {
	header = new RTPHeader(hdr);
	dataSize = _dataSize;
	memcpy(data, packetData, dataSize);
}

RTPPacket::~RTPPacket() {
	delete header;
}

ostream& RTPPacket::toStream(ostream& os) {
	header->toStream(os);
	os.write(data, dataSize);
	return os;
}

string RTPPacket::toString() {
	ostringstream oss;
	toStream(oss);
	return oss.str();
}

