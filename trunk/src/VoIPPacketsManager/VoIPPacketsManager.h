/*
 * VoIPPacketsManager.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef VOIPPACKETSMANAGER_H_
#define VOIPPACKETSMANAGER_H_

#include "debug/debug.h"

class VoIPPacketsManager {
public:
	VoIPPacketsManager();
	virtual ~VoIPPacketsManager();

	/**
	 * Zwraca przez retBuffer pakiet kt�ry jest gotowy do wys�ania.
	 * Zadanie to polega na odczytaniu porcji informacji z pliku audio
	 * lub z pliku z danymi steganograficznymi, opakowaniu jej i zwr�ceniu
	 * przez retBuffer.
	 */
	virtual void getPacketToSend(char** retBuffer, int packetSize) = 0;

	/**
	 * Odbiera pakiet odebrany przez po��czenie VoIP. Zadanie polega
	 * na 'odpakowaniu' pakietu (jesli to konieczne) i zapisaniu
	 * odebranych danych w pami�ci. P�niej dane te b�d� mog�y by�
	 * zapisane w plikach wyj�ciowych.
	 */
	virtual void putReceivedPacket(char** packet, int packetSize) = 0;

};

#endif /* VOIPPACKETSMANAGER_H_ */
