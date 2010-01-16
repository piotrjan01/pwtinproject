/*
 * RTPAgent.h
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#ifndef RTPAGENT_H_
#define RTPAGENT_H_

#include <string>
#include <iostream>
#include <sstream>

#include <stdexcept>

#include <pthread.h>

// Obsługa gniazd
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "RTPPacket.h"

#include "../../debug/debug.h"
#include "../../Util/SynchronizedQueue.h"

using namespace std;

class RTPAgent {
	friend void* RTPAgentReceiverThread(void *);
public:
	/**
	 *	Tworzy nowe gniazdo dla komunikacji RTP
	 */
	RTPAgent();

	virtual ~RTPAgent();

	/**
	 *	Wysyła pakiet RTP po UDP
	 */
	void sendPacket(RTPPacket& rtpPacket);

	/**
	 *	Port lokalny na którym zachodzi komunikacja przez RTP
	 */
	int localPort;

	/**
	 * Ustawia adres IP hosta docelowego
	 */
	void setRemoteIP(string ip);

	/**
	 * Ustawia port hosta docelowego
	 */
	void setRemotePort(int port);

private:
	string remoteIP;
	int remotePort;

	int socketfd;
	sockaddr_in addr_in;

	bool killThread;
	pthread_t thread;
	Semaphore mutex;

	/**
	 * Wysyła pakiet RTP z kolejki pakietów wychodzących po UDP
	 */
	void sendPacket();

	void recvPacket();

	SynchronizedQueue incomingPackets;
	SynchronizedQueue outgoingPackets;
};

void* RTPAgentSenderThread(void *);
void* RTPAgentReceiverThread(void *);

#endif /* RTPAGENT_H_ */
