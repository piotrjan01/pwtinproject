/*
 * StegLACK.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "StegLACK.h"
#include "../Main/Main.h"
#include "../debug/debug.h"
#include <cstdlib>
#include <fstream>

StegLACK::StegLACK(Config* cfg) :
	VoIPPacketsManager(cfg) {
	srand((unsigned) time(0));
	string seq = cfg->stegSequence;
	bool done = false;
	while (!done) {
		int pos = seq.find_first_of(" ");
		if (pos == -1)
			done = true;
		stegSeq.push_back(StegSeqElem(seq.substr(0, pos)));
		seq = seq.substr(pos + 1);
	}

	intervalCount = 0;
	seqPosition = 0;
	seqFireInterval = getRandNumber(config->minStegInterval,
			config->maxStegInterval);
	readStegDataToMem();
	VAR_(3, seqFireInterval);
}

void StegLACK::readStegDataToMem() {
	ifstream f(config->stegDataFile.c_str(), ios::in | ios::binary | ios::ate);
	if (f.is_open()) {
		f.seekg(0, ios::beg);
		while (!f.eof()) {
			char c;
			f.get(c);
			stegData.push_back(c);
		}
		f.close();
	} else
		Main::getMain()->handleError(
				"Can't open the steganography input file: "
						+ config->stegDataFile);
}

vector<char> StegLACK::getStegDataToSend() {
	int size = config->RTPPayloadSize;
	if (stegData.size() < (unsigned) size)
		size = stegData.size();
	vector<char> ret;
	for (int i = 0; i < size; i++)
		ret.push_back(stegData[i]);
	return ret;
}

StegLACK::~StegLACK() {
	// TODO Auto-generated destructor stub
}

RTPPacket& StegLACK::getNextPacket() {
	//we are doing the sequence
	if (intervalCount >= seqFireInterval) {
		StegSeqElem se = stegSeq[seqPosition];
		long delay = getRandNumber(se.intervMin, se.intervMax);
		templatePacket.delay = delay;
		//wysylamy steg packet
		if (se.isStegPacket) {
			//todo: dodac do ret dane steg
		}
		//wysylamy audio packet
		else {
			//todo: dodac do ret dane audio
		}
		seqPosition++;

		//we finished the sequence...
		if ((unsigned) seqPosition == stegSeq.size()) {
			seqPosition = intervalCount = 0;
			seqFireInterval = getRandNumber(config->minStegInterval,
					config->maxStegInterval);
		}
	}
	//we send the normal package
	else {
		intervalCount++;
		//todo: dodac do ret dane audio
	}
	//return prepared packet
	return templatePacket;
}

void StegLACK::putReceivedPacketData(char* data, int dataSize) {
	// TODO czy to w ogle potrzebne? czy jakis watek sie tym nie bedzie zajmowal VoIPModule::processIncomingPackets()
}

StegSeqElem::StegSeqElem(string stegSeq) {
	int semicolpos = stegSeq.find_first_of(':');
	string timing = stegSeq.substr(0, semicolpos);
	string type = stegSeq.substr(semicolpos + 1);
	isStegPacket = (type == "S");
	int dashpos = timing.find_first_of('-');
	if (dashpos == -1) {
		intervMin = intervMax = atoi(timing.c_str());
	} else {
		intervMin = atoi(timing.substr(0, dashpos).c_str());
		intervMax = atoi(timing.substr(dashpos + 1).c_str());

	}
}

/**
 * Losuje liczbe w przedziale domknietym <min, max>
 */
long StegLACK::getRandNumber(long min, long max) {
	return (min + (rand() % (max - min + 1)));
}
