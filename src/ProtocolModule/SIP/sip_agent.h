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

class SIP_Agent {

	friend void * SIP_AgentThread( void * );
	friend class SIP_Message;

	private:

		pthread_t thread;
		Semaphore mutex;

		int sock;
		sockaddr_in address;

		bool killThread;

		std::string proxy, user, pass, tag, callid;
		std::string fromtoline;
		SIP_Authentication authentication;

		unsigned int cseq;

		std::string addressString();

		std::string generateBranch();
		void generateCallID();
		void generateFromTag();

		void sendMessage(SIP_Message &m, std::string address, std::string port);
		void receiveMessage();

		void Register();

	public:

		SIP_Agent(char * localaddr);
		~SIP_Agent();

		/**
		 * Rejestrujemy sie w proxy
		 */
		void Register(std::string user, std::string pass, std::string proxy);

};
#endif

