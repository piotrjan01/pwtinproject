/*
 * NoSteg.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "NoSteg.h"
#include "../Main/Main.h"
#include <fstream>

NoSteg::NoSteg(Config* cfg) :
	VoIPPacketsManager(cfg) {
	templatePacket.delay = config->noStegRTPDelay;

	// TODO inicjalizacja danych na char* ?
	vector<char> data = getAudioDataToSend();
	templatePacket.payload = new char[data.size()];
	for (int i = 0; i < (int) data.size(); i++) {
		templatePacket.payload[i] = data[i];
	}
	templatePacket.payloadSize = data.size();
}

NoSteg::~NoSteg() {
	// TODO Auto-generated destructor stub
}

RTPPacket& NoSteg::getNextPacket() {
	PRNBITS_(4, templatePacket.header->toString());
	templatePacket.header->nextRTPHeader(NARROW_BAND); // changes sequenceNumber and timestamp
	PRNBITS_(4, templatePacket.header->toString());

	templatePacket.delay = config->noStegRTPDelay;
	vector<char> data = getAudioDataToSend();
	templatePacket.payload = new char[data.size()];
	templatePacket.payloadSize = data.size();
	for (int i = 0; i < (int) data.size(); i++) {
		templatePacket.payload[i] = data[i];
	}
	VAR_(4, (int)templatePacket.payloadSize);
	return templatePacket;
}

void NoSteg::putReceivedPacketData(RTPPacket& packet) {
	//TODO: to moze byc za wolne - lepiej trzymac uchwyt do pliku
	ofstream myfile (config->outputAudioFilePath.c_str());
	if (myfile.is_open()) {
		myfile.write(packet.payload, packet.payloadSize);
		myfile.close();
	}
	else Main::getInstance()->handleError("Unable to open output audio data file: "
										+config->outputAudioFilePath);
}
