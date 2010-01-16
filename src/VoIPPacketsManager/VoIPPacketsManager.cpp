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
	readAudioDataFileToMem();
}

VoIPPacketsManager::~VoIPPacketsManager() {
}

/**
 * Returns the data to send. Data size is not greater than RTPPayloadSize defined in Config
 */
vector<char> VoIPPacketsManager::getAudioDataToSend() {
	int size = config->RTPPayloadSize;
	if (audioData.size() < (unsigned)size) size = audioData.size();
	vector<char> ret;
	for (int i=0; i<size; i++) ret.push_back(audioData[i]);
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
	else Main::getMain()->handleError("Can't open the audio input file: "+config->audioFilePath);
}