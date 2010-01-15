/*
 * VoIPModule.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "VoIPModule.h"
#include "../VoIPPacketsManager/NoSteg.h"
#include "../VoIPPacketsManager/StegLACK.h"

VoIPModule::VoIPModule(Config* _config) : config(*_config) {
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
 * Wysy�amy dane z pliku audio do wskazanego hosta zestawiaj�c najpierw po��czenia VoIP.
 */
void VoIPModule::doSending() {
	initializeConnection();
	sipAgent->Call(config.calleeID, config.callPort);

}

/**
 * Czekamy na po��czenie, odbieramy je i zapisujemy dane audio do wskazanego pliku.
 */
void VoIPModule::doReceiving() {
	initializeConnection();
	// TODO
	//agent->Answer(...);
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
