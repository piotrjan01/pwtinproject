/*
 * NoSteg.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "NoSteg.h"
#include "../Main/Main.h"
#include "../Util/FileOperations.h"
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

	timeSinceLastQueueRead.start();

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

	//if there is space in a queue
		if ((int)incQueue.size() < config->incQueueSize) {
			PRN_(1, "receiving package: putting it to incoming queue");
			incQueue.push_back(packet);
		}
		//if not, it may be steg packet. normally dropped by VoIP client.
		else {
			PRN_(1, "receiving package: no space in incoming queue, dropping package.");
		}

		//if it is time to read something from the queue:
		VAR_(3, timeSinceLastQueueRead.seeTime());
		VAR_(3, config->incQueueReadInterval);
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
