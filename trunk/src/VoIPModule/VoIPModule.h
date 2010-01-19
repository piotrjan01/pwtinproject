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

/**
 * \~
 * \~polish
 * Klasa pośrednicząca między pozostałymi modułami aplikacji VoIP.
 *
 * Nawiązuje połączenie korzystając z SIP i RTP, następnie pobiera pakiety
 * od @ref VoIPPacketsManager i przekazuje do wysłania.
 * W międzyczasie odbiera pakiety i przekazuje je do przetworzenia również do VoIPPacketsManager.
 *
 * \~english
 * A controlling class that administers and mediates other modules of VoIP application.
 *
 * It makes a connection using SIP and RTP protocols, and then gets packets
 * from @ref VoIPPacketsManager.
 * In the meantime it receives packets and pushes them to the @ref VoIPPacketsManager.
 *
 * \~
 */
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
