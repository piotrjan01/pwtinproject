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
	payload = NULL;
	payloadSize = 0;
}

RTPPacket::RTPPacket(const RTPPacket& another) {
	delay = another.delay;
	header = new RTPHeader(*another.header);
	payloadSize = another.payloadSize;
	payload = new char[another.payloadSize];
	memcpy(payload, another.payload, another.payloadSize);
}

RTPPacket::RTPPacket(char* packet, int _dataSize) {
	if ((unsigned) _dataSize < RTPHeader::SIZE_IN_BYTES) {
		throw DeserializationException();
	}
	header = new RTPHeader(packet);
	payloadSize = _dataSize - RTPHeader::SIZE_IN_BYTES;
	payload = new char[payloadSize];
	memcpy(payload, packet + RTPHeader::SIZE_IN_BYTES, payloadSize);
}

RTPPacket& RTPPacket::operator=(const RTPPacket& another) {
	if (this == &another) {
		return *this;
	}
	delay = another.delay;
	*header = *another.header;
	payloadSize = another.payloadSize;
	delete[] payload;
	payload = new char[another.payloadSize];
	memcpy(payload, another.payload, another.payloadSize);
	return *this;
}

RTPPacket::RTPPacket(const RTPHeader& hdr, char* packetData, int _dataSize) {
	header = new RTPHeader(hdr);
	payloadSize = _dataSize;
	payload = new char[payloadSize];
	memcpy(payload, packetData, payloadSize);
}

RTPPacket::~RTPPacket() {
	delete header;
	delete[] payload;
}

ostream& RTPPacket::toStream(ostream& os) {
	header->toStream(os);
	os.write(payload, payloadSize);
	return os;
}

string RTPPacket::toString() {
	ostringstream oss;
	toStream(oss);
	return oss.str();
}

