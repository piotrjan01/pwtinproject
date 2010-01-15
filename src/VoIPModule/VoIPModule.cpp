/*
 * VoIPModule.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizdała
 */

#include "VoIPModule.h"
#include "../VoIPPacketsManager/NoSteg.h"
#include "../VoIPPacketsManager/StegLACK.h"

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
	// TODO Auto-generated destructor stub
	delete &sipAgent;
	delete &packetsManager;
}

/**
 * Wysy�amy dane z pliku audio do wskazanego hosta zestawiając najpierw połączenia VoIP.
 */
void VoIPModule::doSending() {
	initializeConnection();
	// tutaj odbieramy od sipAgent dane potrzebne dla RTPAgent (ewentualnie też RTCPAgent)
	*callInfo = sipAgent->Call(config.calleeID, config.callPort);

	//	rtpAgent = new RTPAgent(senderPort, receiverIP, receiverPort);

	// TODO pseudokod poniżej
	//	while(sipAgent.isConnected()) {
	//	while (true) {
	//		delay = packetsManager->getDelay();
	//		packetsManager->getPacketToSend(rtpPacket);
	//		sleep(delay);
	//		rtpAgent->sendPacket(rtpPacket);
	//		// TODO
	//		// packets = rtpAgent->getPackets();
	//	}
}

/**
 * Czekamy na połączenie, odbieramy je i zapisujemy dane audio do wskazanego pliku.
 */
void VoIPModule::doReceiving() {
	initializeConnection();

	// tutaj odbieramy od sipAgent dane potrzebne dla RTPAgent (ewentualnie też RTCPAgent)
	*callInfo = sipAgent->Answer(config.callPort);

	// TODO
}

/**
 * W zaleznosci od config-a rozpoczyna dzwonienie lub nasluchiwanie
 */
void VoIPModule::connect() {
	if (config.weAreCalling) {
		doSending();
	} else {
		doReceiving();
	}
}

void VoIPModule::initializeConnection() {
	sipAgent->Register(config.myUser, config.myPass, config.SIPProxyIP);
}
