/*
 * sip_agent.h 
 * Agent SIP - glowna klasa
 */

#ifndef SIP_AGENT_H
#define SIP_AGENT_H

#include <string>
#include <sstream>
#include <vector>

#include <pthread.h> 

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "semaphore.h"

#include "sip_message.h"
#include "sip_authentication.h"

struct CallInfo {
	std::string callID;
	std::string remoteIP;
	unsigned short remotePort;
};



struct SIP_Info {
	unsigned int cseq;
	std::string callid;
	SIP_Authentication authentication;
};

enum Waitfor { NONE, REGISTER, CALL, ANSWER };

class SIP_Agent {

	friend void * SIP_AgentThread( void * );
	friend class SIP_Message;

	private:

		// Threading
		pthread_t thread;
		Semaphore mutex;

		// Semafor pozwalajacy na blokowanie metod
		Semaphore wait;
		Waitfor waitingfor;

		// Socket
		int sock;

		// Adres lokalny
		sockaddr_in address;

		// Ustawione na true zabija
		bool killThread;

		// Podstawowe informacje o uzytkowniku
		std::string proxy, user, pass, tag, branch;
		std::string fromtoline;

		// Sesja rejestrujaca
		SIP_Info registration;

		// Sesja nawiazujaca
		SIP_Info invite;

		// porty RTP
		unsigned short localRtpPort, remoteRtpPort;
		
		// identyfikator oraz ip rozmówcy
		std::string partnerID, remoteRtpAddress;

		// czy czekamy na telefon?
		bool waitingForCall;

		std::string addressString();

		std::string generateBranch();
		std::string generateCallID();
		void generateFromTag();

		void sendMessage(SIP_Message &m, std::string address, std::string port);
		void receiveMessage();

		void replyAck(SIP_Message &m, std::string information = "");
		void replyToOptions(SIP_Message &m);
		void replyRinging(SIP_Message &m);

		void answerCall(SIP_Message &m);

		void Register();
		void Call();
		

	public:

		SIP_Agent(std::string localaddr);
		~SIP_Agent();

		void Register(std::string user, std::string pass, std::string proxy);

		CallInfo Answer(unsigned short port);
		CallInfo Call(std::string id, unsigned short port);

};
#endif

