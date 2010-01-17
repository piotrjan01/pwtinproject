/*
 * VoIPModule.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef VOIPMODULE_H_
#define VOIPMODULE_H_

#include <string>
#include <stdexcept>

#include <queue>

#include "debug/debug.h"

#include "../Main/Config.h"

#include "../ProtocolModule/SIP/sip_agent.h"
#include "../ProtocolModule/RTP/RTPAgent.h"
#include "../ProtocolModule/RTP/RTPPacket.h"

#include "../VoIPPacketsManager/VoIPPacketsManager.h"
#include "../VoIPPacketsManager/NoSteg.h"
#include "../VoIPPacketsManager/StegLACK.h"

using namespace std;

class VoIPModule {
public:
	VoIPModule(Config* config);
	virtual ~VoIPModule();

	void connect();

private:
	void doSending();
	void doReceiving();
	void doTransport();

	void processIncomingPackets();

	Config& config;

	SIP_Agent* sipAgent;
	RTPAgent* rtpAgent;

	VoIPPacketsManager* packetsManager;
	CallInfo callInfo;
};

#endif /* VOIPMODULE_H_ */
