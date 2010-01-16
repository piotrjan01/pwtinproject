/*
 * NoSteg.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef NOSTEG_H_
#define NOSTEG_H_

#include "VoIPPacketsManager.h"

class NoSteg: public VoIPPacketsManager {
public:
	NoSteg();
	virtual ~NoSteg();

	virtual RTPPacket getNextPacket();
	virtual void putReceivedPacketData(char* data, int dataSize);
};

#endif /* NOSTEG_H_ */
