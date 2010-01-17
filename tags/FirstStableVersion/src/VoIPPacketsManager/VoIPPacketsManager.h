/*
 * VoIPPacketsManager.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef VOIPPACKETSMANAGER_H_
#define VOIPPACKETSMANAGER_H_

#include <vector>

#include "../ProtocolModule/RTP/RTPPacket.h"
#include "debug/debug.h"
#include "../Main/Config.h"

class VoIPPacketsManager {
public:

	Config* config;

	int lastReadByte;

	vector<char> audioData;

	VoIPPacketsManager(Config *cfg);
	virtual ~VoIPPacketsManager();

	/**
	 * Zwraca pakiet który jest gotowy do wysłania.
	 */
	virtual RTPPacket& getNextPacket() = 0;

	virtual void putReceivedPacketData(RTPPacket& packet) = 0;

	virtual vector<char> getAudioDataToSend();

	void readAudioDataFileToMem();
protected:
	RTPPacket templatePacket;
};

#endif /* VOIPPACKETSMANAGER_H_ */
