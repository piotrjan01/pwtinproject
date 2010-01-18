/*
 * NoSteg.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef NOSTEG_H_
#define NOSTEG_H_

#include "VoIPPacketsManager.h"
#include "../Debug/Debug.h"

/**
 * \~
 * \~polish
 * Implementacja @ref VoIPPacketsManager wysyłająca wyłącznie dane audio.
 *
 * Dzieli dane audio na pakiety i wysyła je w równych odstępach czasu.
 * Jest nieświadoma transmisji pakietów steganograficznych.
 *
 * \~english
 * Implementation of @ref VoIPPacketsManager that sends only audio data.
 *
 * Divides audio data into packets and sends them in equal time spans.
 * <code>NoSteg</code> is completely unaware of possible steganographic
 * data packets that it receives.
 * \~
 * @see StegLACK
 */
class NoSteg: public VoIPPacketsManager {
	static const int DEFAULT_PACKET_DELAY = TIMESTAMP_INTERVAL_8KHZ;
public:
	NoSteg(Config* cfg);
	virtual ~NoSteg();

	virtual RTPPacket& getNextPacket();
	virtual void putReceivedPacketData(RTPPacket& packet);
};

#endif /* NOSTEG_H_ */
