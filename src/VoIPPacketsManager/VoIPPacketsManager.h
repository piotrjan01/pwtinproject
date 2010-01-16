/*
 * VoIPPacketsManager.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef VOIPPACKETSMANAGER_H_
#define VOIPPACKETSMANAGER_H_

#include "../ProtocolModule/RTP/RTPPacket.h"

#include "debug/debug.h"

class VoIPPacketsManager {
public:
	VoIPPacketsManager();
	virtual ~VoIPPacketsManager();

	/**
	 * Zwraca pakiet który jest gotowy do wysłania.
	 * Zadanie to polega na odczytaniu porcji informacji z pliku audio
	 * lub z pliku z danymi steganograficznymi, opakowaniu jej i zwróceniu.
	 */
	virtual RTPPacket getNextPacket() = 0;

	/**
	 * Odbiera pakiet odebrany przez połączenie VoIP. Zadanie polega
	 * na 'odpakowaniu' pakietu (jesli to konieczne) i zapisaniu
	 * odebranych danych w pamięci. Póniej dane te będą mogły być
	 * zapisane w plikach wyjściowych.
	 */
	virtual void putReceivedPacketData(char* data, int dataSize) = 0;
};

#endif /* VOIPPACKETSMANAGER_H_ */
