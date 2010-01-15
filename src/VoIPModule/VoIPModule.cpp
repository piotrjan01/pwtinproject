/*
 * VoIPModule.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "VoIPModule.h"

VoIPModule::VoIPModule(Config* _config) : config(*_config) {
	agent = new SIP_Agent(config.localIP);
}

VoIPModule::~VoIPModule() {
	// TODO Auto-generated destructor stub
}

/**
 * Wysy�amy dane z pliku audio do wskazanego hosta zestawiaj�c najpierw po��czenia VoIP.
 */
void VoIPModule::doSending() {
}

/**
 * Czekamy na po��czenie, odbieramy je i zapisujemy dane audio do wskazanego pliku.
 */
void VoIPModule::doReceiving() {

}

void VoIPModule::connect() {
	if(config.weAreCalling) {
		doSending();
	} else {
		doReceiving();
	}
}

