/*
 * NoSteg.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "NoSteg.h"

NoSteg::NoSteg(Config* cfg) : VoIPPacketsManager(cfg) {
	templatePacket.header->sequenceNumber = RTPHeader::generateSequenceNumber();
	VAR_(2, templatePacket.header->sequenceNumber);
	templatePacket.header->timestamp = RTPHeader::generateTimestamp();
	VAR_(2, (int) templatePacket.header->timestamp);
	templatePacket.header->ssrc = RTPHeader::generateSSRC();
	VAR_(2, (int) templatePacket.header->ssrc);
	PRNBITS_(2, templatePacket.header->toString());
	// TODO inicjalizacja danych -- wczytanie
}

NoSteg::~NoSteg() {
	// TODO Auto-generated destructor stub
}

RTPPacket NoSteg::getNextPacket() {
	PRNBITS_(2, templatePacket.header->toString());
	templatePacket.header->nextRTPHeader(NARROW_BAND); // changes sequenceNumber and timestamp
	PRNBITS_(2, templatePacket.header->toString());

	// TODO
	RTPPacket packet(templatePacket);
	packet.delay = DEFAULT_PACKET_DELAY;
	packet.data = NULL;
	packet.dataSize = 0;
	return packet;
}

void NoSteg::putReceivedPacketData(char* data, int dataSize) {
	// TODO
}
