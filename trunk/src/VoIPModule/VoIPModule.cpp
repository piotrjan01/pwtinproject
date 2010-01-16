/*
 * VoIPModule.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizdała
 */

#include "VoIPModule.h"

VoIPModule::VoIPModule(Config* _config) :
	config(*_config) {
	sipAgent = new SIP_Agent(config.localIP);
	if (config.doSteg) {
		packetsManager = new StegLACK();
	} else {
		packetsManager = new NoSteg();
	}
}

VoIPModule::~VoIPModule() {
	delete &sipAgent;
	delete &packetsManager;
}

static const long USECONDS_IN_A_MILISECOND = 1000;
/**
 * Wysyłamy dane z pliku audio do wskazanego hosta zestawiając najpierw połączenia VoIP.
 */
void VoIPModule::doSending() {
	// tutaj odbieramy od sipAgent dane potrzebne dla RTPAgent (ewentualnie też RTCPAgent)
	callInfo = sipAgent->Call(config.calleeID, rtpAgent->localPort);
	if (callInfo.remotePort == 0) {
		throw runtime_error("Call failed");
	}

	rtpAgent->setRemoteIP(callInfo.remoteIP);
	rtpAgent->setRemotePort(callInfo.remotePort);

	RTPPacket rtpPacket;
	while (sipAgent->connected()) {
		rtpPacket = packetsManager->getNextPacket();
		// TODO właściwie to ma być <delay> milisekund różnicy pomiędzy kolejnymi wysłaniami pakietów
		// a poniżej jest <delay> milisekund przerwy + obliczenia (pobranie pakietu itp., przygotowanie danych)
		usleep(USECONDS_IN_A_MILISECOND * rtpPacket.delay);
		rtpAgent->sendPacket(rtpPacket);
		processIncomingPackets(); // można wyrzucić przed pętlę -- musi to być wtedy wątek przetwarzający pakiety, gdy kolejka nie jest pusta
	}
	sipAgent->Disconnect();
}

/**
 * Czekamy na połączenie, odbieramy je i zapisujemy dane audio do wskazanego pliku.
 */
void VoIPModule::doReceiving() {
	// tutaj odbieramy od sipAgent dane potrzebne dla RTPAgent (ewentualnie też RTCPAgent)
	//	CallInfo callInfo = sipAgent->Answer(rtpAgent->port);

	// TODO
}

/**
 * TODO
 *
 *	Osobny wątek przetwarza pakiety przychodzące
 */
void VoIPModule::processIncomingPackets() {
	//	while(rtpAgent->hasNextPacket()) {
	//		 processPacket(rtpAgent->getReceivedPacket());
	//	}
}

/**
 * W zaleznosci od configa rozpoczyna dzwonienie lub nasluchiwanie
 */
void VoIPModule::connect() {
	sipAgent->Register(config.myUser, config.myPass, config.SIPProxyIP, config.proxyPort);

	rtpAgent = new RTPAgent();

	if (config.weAreCalling) {
		doSending();
	} else {
		doReceiving();
	}
}
