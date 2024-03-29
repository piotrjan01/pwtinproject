/*
 * @file VoIPPacketsManager.h
 *
 * @date created 2010-01-14
 * @author Piotr Gwizdała
 */

#ifndef VOIPPACKETSMANAGER_H_
#define VOIPPACKETSMANAGER_H_

#include <vector>

#include "../ProtocolModule/RTP/RTPPacket.h"
#include "../Debug/Debug.h"
#include "../Main/Config.h"

/**
 * \~polish
 * Klasa abstrakcyjna. Dzieli dane na pakiety i wysyła je z odpowiednimi opóźnieniami
 * korzystając z @ref RTPAgent.
 *
 * Dotychczas wykonane są dwie implementacje: @ref StegLACK i @ref NoSteg.
 * Odpowiada również za przetwarzanie danych w pakietach przychodzących.
 * Sposób działania zależy od implementacji.
 *
 * \~english
 * Abstract class responsible for dividing data into packets and sending them
 * with definite delays with the aid of @ref RTPAgent.
 *
 * There are to implementations so far:  @ref StegLACK and @ref NoSteg.
 * This module is also responsible for handling the processing of data from
 * incoming packets.
 * Exact behavior is implementation-dependent.
 *
 * \~
 * @see RTPAgent
 * @see StegLACK
 * @see NoSteg
 */
class VoIPPacketsManager {
public:

	Config* config;

	int lastReadByte;

	vector<char> audioData;

	VoIPPacketsManager(Config *cfg);
	virtual ~VoIPPacketsManager();

	/**
	 * Zwraca pakiet który jest gotowy do wysłania.
	 */
	virtual RTPPacket& getNextPacket() = 0;

	/**
	 * Obsługuje otrzymany pakiet. (np. zapisuje do pliku lub rozpoznaje
	 * dane steg. w zależności od implementacji).
	 */
	virtual void putReceivedPacketData(RTPPacket& packet) = 0;

protected:

	virtual vector<char> getAudioDataToSend();

	void readAudioDataFileToMem();


	RTPPacket templatePacket;
};

#endif /* VOIPPACKETSMANAGER_H_ */
