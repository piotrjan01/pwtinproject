/*
 * VoIPPacketsManager.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#ifndef VOIPPACKETSMANAGER_H_
#define VOIPPACKETSMANAGER_H_

#include "debug/debug.h"

class VoIPPacketsManager {
public:
	VoIPPacketsManager();
	virtual ~VoIPPacketsManager();

	void getPacketToSend(char** retBuffer, int packetSize);

	void putReceivedPacket(char** packet, int packetSize);

};

#endif /* VOIPPACKETSMANAGER_H_ */
