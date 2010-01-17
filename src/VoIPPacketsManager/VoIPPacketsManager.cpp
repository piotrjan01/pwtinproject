/*
 * VoIPPacketsManager.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "VoIPPacketsManager.h"
#include "../Main/Main.h"
#include <fstream>
#include "../debug/debug.h"

VoIPPacketsManager::VoIPPacketsManager(Config* cfg) {
	config = cfg;
	lastReadByte = 0;
	readAudioDataFileToMem();

	templatePacket.header->sequenceNumber = RTPHeader::generateSequenceNumber();
	VAR_(2, templatePacket.header->sequenceNumber);
	templatePacket.header->timestamp = RTPHeader::generateTimestamp();
	VAR_(2, (int) templatePacket.header->timestamp);
	templatePacket.header->ssrc = RTPHeader::generateSSRC();
	VAR_(2, (int) templatePacket.header->ssrc);
	PRNBITS_(2, templatePacket.header->toString());
}

VoIPPacketsManager::~VoIPPacketsManager() {
}

/**
 * Returns the data to send. Data size is not greater than RTPPayloadSize defined in Config
 */
vector<char> VoIPPacketsManager::getAudioDataToSend() {
	int size = config->RTPPayloadSize;
	vector<char> ret;
	for (int i=1; i<size; i++) ret.push_back(audioData[(++lastReadByte) % audioData.size()]);
	lastReadByte = lastReadByte % audioData.size();
	return ret;
}

void VoIPPacketsManager::readAudioDataFileToMem() {
	ifstream f(config->audioFilePath.c_str(), ios::in|ios::binary|ios::ate);
	if (f.is_open()) {
		f.seekg(0, ios::beg);
		while (! f.eof() ) {
			char c;
			f.get(c);
			audioData.push_back(c);
		}
		f.close();
	}
	else Main::getInstance()->handleError("Can't open the audio input file: "+config->audioFilePath);
}
