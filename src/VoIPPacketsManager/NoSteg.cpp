/*
 * NoSteg.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "NoSteg.h"

//TODO: dodac to do config?
#define NO_STEG_DELAY 10

NoSteg::NoSteg(Config* cfg) : VoIPPacketsManager(cfg) {
}

NoSteg::~NoSteg() {
	// TODO Auto-generated destructor stub
}

RTPPacket NoSteg::getNextPacket() {

	//TODO: albo przechodzimy na vector<char> wszedzie, albo zostaje to:
	//(vector<char> da nam pewnosc ze sie gdzies nie walnelismy ze wskaznikami i destruktorami)
	vector<char> data = getAudioDataToSend();
	char* chardata = new char[data.size()];
	for (int i=0; i<(int)data.size(); i++) chardata[i] = data[i];

	//TODO: skad mam wziac header'a ??
	RTPHeader head;
	RTPPacket ret(head, chardata, data.size());
	ret.delay = NO_STEG_DELAY;
	return ret;
}

void NoSteg::putReceivedPacketData(char* data, int dataSize) {
	// TODO ??
}

