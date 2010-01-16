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

	virtual RTPPacket getNextPacket();
	virtual void putReceivedPacketData(char* data, int dataSize);
};

#endif /* STEGLACK_H_ */
