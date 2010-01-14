/*
 * VoIPPacketsManager.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#include "VoIPPacketsManager.h"

/**
 * TODO: jakieœ dane potrzebne przy konstrukcji!
 */
VoIPPacketsManager::VoIPPacketsManager() {
	// TODO Auto-generated constructor stub

}

VoIPPacketsManager::~VoIPPacketsManager() {
	// TODO Auto-generated destructor stub
}


/**
 * Zwraca przez retBuffer pakiet który jest gotowy do wys³ania.
 * Zadanie to polega na odczytaniu porcji informacji z pliku audio
 * lub z pliku z danymi steganograficznymi, opakowaniu jej i zwróceniu
 * przez retBuffer.
 */
void getPacketToSend(char** retBuffer, int packetSize) {

}

/**
 * Odbiera pakiet odebrany przez po³¹czenie VoIP. Zadanie polega
 * na 'odpakowaniu' pakietu (jesli to konieczne) i zapisaniu
 * odebranych danych w pamiêci. PóŸniej dane te bêd¹ mog³y byæ
 * zapisane w plikach wyjœciowych.
 */
void putReceivedPacket(char** packet, int packetSize) {

}
