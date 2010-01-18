/**
 * @file VoIPModule.h
 *
 * @date created 2010-01-14
 * @author: Piotr Gwizda�a
 */
#ifndef VOIPMODULE_H_
#define VOIPMODULE_H_

#include <string>
#include <stdexcept>

#include <queue>

#include "../Debug/Debug.h"

#include "../Main/Config.h"
#include "../Main/Main.h"

#include "../ProtocolModule/SIP/SIPAgent.h"
#include "../ProtocolModule/RTP/RTPAgent.h"
#include "../ProtocolModule/RTP/RTPPacket.h"

#include "../VoIPPacketsManager/VoIPPacketsManager.h"
#include "../VoIPPacketsManager/NoSteg.h"
#include "../VoIPPacketsManager/StegLACK.h"

#include "../Util/Observer/Observer.h"
#include "../Util/Timer.h"

using namespace std;

class VoIPModule : public Observer {
public:
	VoIPModule(Config* config);
	virtual ~VoIPModule();

	void connect();

	//@Override
	void update();
private:
	void processIncomingPackets();

	void doSending();
	void doReceiving();
	void doTransport();

	Config& config;

	SIPAgent* sipAgent;
	RTPAgent* rtpAgent;

	VoIPPacketsManager* packetsManager;
	CallInfo callInfo;
};

#endif /* VOIPMODULE_H_ */
