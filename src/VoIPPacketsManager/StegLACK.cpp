/*
 * StegLACK.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "StegLACK.h"
#include "../Main/Main.h"
#include "../Debug/Debug.h"
#include "../Util/FileOperations.h"
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

	timeSinceLastQueueRead.start();

	intervalCount = 0;
	seqPosition = 0;
	seqFireInterval = getRandNumber(config->minStegInterval,
			config->maxStegInterval);
	stegTransferDone = false;
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
		Main::getInstance()->handleError(
				"Can't open the steganography input file: "
						+ config->stegDataFile);
}

vector<char> StegLACK::getStegDataToSend() {
	int size = config->RTPPayloadSize;
	vector<char> ret;
	string flag = STEG_DATA_FLAG;
	for (int i=0; i<STEG_DATA_FLAG_SIZE; i++) {
		ret.push_back(flag[i]);
	}
	for (int i=STEG_DATA_FLAG_SIZE; i<size; i++) {
		lastReadStegByte++;
		if (lastReadStegByte<(int)stegData.size())
			ret.push_back(stegData[lastReadStegByte]);
		else ret.push_back(' ');
	}
	if (lastReadStegByte>=(int)stegData.size()) stegTransferDone = true;
	return ret;
}

StegLACK::~StegLACK() {

}

RTPPacket& StegLACK::getNextPacket() {

	vector<char> payloadData;

	//we are doing the sequence
	if ( ! stegTransferDone && intervalCount >= seqFireInterval) {
		StegSeqElem se = stegSeq[seqPosition];
		long delay = getRandNumber(se.intervMin, se.intervMax);
		templatePacket.delay = delay;
		//wysylamy steg packet
		if (se.isStegPacket && ! stegTransferDone) {
			payloadData = getStegDataToSend();
			PRN_(1, "in steg sequence: sending packet: steg");
			VAR_(1, delay);
		}
		//wysylamy audio packet
		else {
			//dodajemy audio do pakietu:
			payloadData = getAudioDataToSend();
			PRN_(1, "in steg sequence: sending packet: audio");
			VAR_(1, delay);
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
		templatePacket.delay = config->noStegRTPDelay;
		PRN_(1, "sending normal packet");
		//dodajemy audio do pakietu:
		payloadData = getAudioDataToSend();
	}

	templatePacket.payload = new char[payloadData.size()];
	templatePacket.payloadSize = payloadData.size();
	for (int i = 0; i < (int) payloadData.size(); i++) {
		templatePacket.payload[i] = payloadData[i];
	}

	//return prepared packet
	return templatePacket;
}

void StegLACK::saveIfStegPacket(RTPPacket &p) {
	//we verify if it is flagged as steg packet:
	string flag = STEG_DATA_FLAG;
	if (p.payloadSize < STEG_DATA_FLAG_SIZE) return;
	for (int i=0; i<STEG_DATA_FLAG_SIZE; i++) {
		if (p.payload[i] != flag[i]) {
			PRN_(1, "no steg packet flag present: dropping packet");
			return;
		}
	}

	PRN_(1, "steg packet! saving to file...");
	FileOperations::writeToFile(config->outputStegDataFile,
			(p.payload+STEG_DATA_FLAG_SIZE), p.payloadSize-STEG_DATA_FLAG_SIZE);

}

void StegLACK::putReceivedPacketData(RTPPacket& packet) {

	//if there is space in a queue
	if ((int)incQueue.size() < config->incQueueSize) {
		PRN_(1, "receiving package: putting it to incoming queue");
		incQueue.push_back(packet);
	}
	//if not, it may be steg packet. normally dropped by VoIP client.
	else {
		saveIfStegPacket(packet);
	}

	//if it is time to read something from the queue:
	VAR_(1, timeSinceLastQueueRead.seeTime());
	VAR_(1, config->incQueueReadInterval);
	if (timeSinceLastQueueRead.seeTime() > config->incQueueReadInterval) {
		timeSinceLastQueueRead.start(); //restart timer
		PRN_(1, "its time to read all packages from incoming queue...");
		if (incQueue.size() > 0) {
			VAR_(1, (int)incQueue.size());
			int plSize = 0;
			for (int i=0; i<(int)incQueue.size(); i++) {
				plSize += incQueue[i].payloadSize;
			}
			char* plData = new char[plSize];
			plSize = 0;
			for (int i=0; i<(int)incQueue.size(); i++) {
				memcpy((plData+plSize), incQueue[i].payload, incQueue[i].payloadSize);
				plSize += incQueue[i].payloadSize;
			}
			FileOperations::writeToFile(config->outputAudioFilePath, plData, plSize);
			incQueue.clear();
		}
		else {
			PRN_(1, "incoming queue is empty.");
		}

	}

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
