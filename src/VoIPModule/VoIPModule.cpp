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
		packetsManager = new StegLACK(_config);
	} else {
		packetsManager = new NoSteg(_config);
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

	doTransport();
}

/**
 * Czekamy na połączenie, odbieramy je i zapisujemy dane audio do wskazanego pliku.
 */
void VoIPModule::doReceiving() {
	// tutaj odbieramy od sipAgent dane potrzebne dla RTPAgent (ewentualnie też RTCPAgent)
	callInfo = sipAgent->Answer(rtpAgent->localPort);
	if (callInfo.remotePort == 0) {
		throw runtime_error("Call failed");
	}
	rtpAgent->setRemoteIP(callInfo.remoteIP);
	rtpAgent->setRemotePort(callInfo.remotePort);

	doTransport();
}

void VoIPModule::doTransport() {
	RTPPacket rtpPacket;
	while (sipAgent->connected() && ! Main::getInstance()->isHangUp()) {
		PRN_(3, "VoIP: getNextPacket...");
		rtpPacket = packetsManager->getNextPacket();
		PRN_(3, "VoIP: OK have packet");
		PRNBITS_(4, rtpPacket.header->toString());

		// TODO właściwie to ma być <delay> milisekund różnicy pomiędzy kolejnymi wysłaniami pakietów
		// a poniżej jest <delay> milisekund przerwy + obliczenia (pobranie pakietu itp., przygotowanie danych)
		VAR_(2, (int) rtpPacket.delay);
		usleep(USECONDS_IN_A_MILISECOND * rtpPacket.delay);
		PRN_(3, "VoIP: sendPacket");
		rtpAgent->sendPacket(rtpPacket);
	}
	sipAgent->Disconnect();
}

//@Override
void VoIPModule::update() {
	processIncomingPackets();
}

/**
 * TODO
 *
 *	Osobny wątek przetwarza pakiety przychodzące
 *	Uwaga: będziemy musieli zasymulować bufor klienta i rozpoznawać pakiety które bylyby
 *	normalnie drop-owane, czyli prawdopodobnie nasze dane steganograficzne.
 */
void VoIPModule::processIncomingPackets() {
	PRN_(4, "VoIP: processIncomingPackets...");
	while (rtpAgent->hasReceivedPacket()) {
		RTPPacket& packet = rtpAgent->getReceivedPacket();
		packetsManager->putReceivedPacketData(packet.data, packet.dataSize);
	}
}

/**
 * W zaleznosci od configa rozpoczyna dzwonienie lub nasluchiwanie
 */
void VoIPModule::connect() {
	if (sipAgent->Register(config.myUser, config.myPass, config.SIPProxyIP,
			config.SIPProxyPort)) {
		PRN("SIP Register successfully done")
	} else {
		PRN("SIP Register failed")
		return; // TODO throw exception
	}

	rtpAgent = new RTPAgent();

	if (config.weAreCalling) {
		doSending();
	} else {
		doReceiving();
	}
	sipAgent->Unregister();
}
