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
	/**
	 * TODO: jakieœ dane potrzebne przy konstrukcji!
	 */
	VoIPPacketsManager();
	virtual ~VoIPPacketsManager();

	/**
	 * Zwraca przez retBuffer pakiet który jest gotowy do wys³ania.
	 * Zadanie to polega na odczytaniu porcji informacji z pliku audio
	 * lub z pliku z danymi steganograficznymi, opakowaniu jej i zwróceniu
	 * przez retBuffer.
	 */
	void getPacketToSend(char** retBuffer, int packetSize);

	/**
	 * Odbiera pakiet odebrany przez po³¹czenie VoIP. Zadanie polega
	 * na 'odpakowaniu' pakietu (jesli to konieczne) i zapisaniu
	 * odebranych danych w pamiêci. PóŸniej dane te bêd¹ mog³y byæ
	 * zapisane w plikach wyjœciowych.
	 */
	void putReceivedPacket(char** packet, int packetSize);

};

#endif /* VOIPPACKETSMANAGER_H_ */
