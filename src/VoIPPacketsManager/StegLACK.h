/*
 * StegLACK.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef STEGLACK_H_
#define STEGLACK_H_

#include "VoIPPacketsManager.h"

class StegLACK: public VoIPPacketsManager {
public:
	StegLACK();
	virtual ~StegLACK();
	virtual void getPacketToSend(char** retBuffer, int packetSize);
	virtual void putReceivedPacket(char** packet, int packetSize);
};

#endif /* STEGLACK_H_ */
