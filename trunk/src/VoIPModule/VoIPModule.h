/*
 * VoIPModule.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef VOIPMODULE_H_
#define VOIPMODULE_H_

#include <string>
#include "debug/debug.h"
#include "../Main/Config.h"
#include "../ProtocolModule/SIP/sip_agent.h"
#include "../VoIPPacketsManager/VoIPPacketsManager.h"

using namespace std;

class VoIPModule {
public:
	VoIPModule(Config* config);
	virtual ~VoIPModule();

	void connect();

private:
	void doSending();

	void doReceiving();

	void initializeConnection();

	Config& config;

	SIP_Agent* sipAgent;
	VoIPPacketsManager* packetsManager;

};

#endif /* VOIPMODULE_H_ */
