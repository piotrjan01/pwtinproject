/*
 * NoSteg.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef NOSTEG_H_
#define NOSTEG_H_

#include "VoIPPacketsManager.h"
#include "../debug/debug.h"

class NoSteg: public VoIPPacketsManager {
	// temporary only
	static const int DEFAULT_PACKET_DELAY = TIMESTAMP_INTERVAL_8KHZ;
public:

	NoSteg(Config* cfg);
	virtual ~NoSteg();

	virtual RTPPacket& getNextPacket();
	virtual void putReceivedPacketData(char* data, int dataSize);
};

#endif /* NOSTEG_H_ */
