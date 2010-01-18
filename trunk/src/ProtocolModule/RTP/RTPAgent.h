/**
 * @file RTPAgent.h
 *
 *
 * @date created 2010-01-16
 * @author: Marek
 * @see RTPHeader.h
 * @see RTPPacket.h
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

#include "../../Debug/Debug.h"
#include "../../Util/SynchronizedQueue.h"
#include "../../Util/Net.h"
#include "../../Util/Observer/Observable.h"

using namespace std;

/**
 * Class responsible for sending and receiving of RTP (and RTCP*) packets.
 */
class RTPAgent: public Observable {
	friend void* RTPAgentReceiverThread(void *);
public:
	/**
	 *	Creates new sockets for RTP, RTCP communication.
	 */
	RTPAgent();

	virtual ~RTPAgent();

	/**
	 *	Sends a RTP packet using UDP.
	 *
	 *	Effectively adds the RTP packet to the outgoing packets queue.
	 *
	 *	@param rtpPacket Packet to be sent.
	 */
	void sendPacket(RTPPacket& rtpPacket);

	/**
	 * Checks whether there are packets in the incoming packets queue.
	 *
	 * @return <code>true</code> if there is at least one received RTP packet in the incoming packets queue, <code>false</code> otherwise
	 */
	bool hasReceivedPacket();

	/**
	 * Takes a RTP packet from the queue.
	 *
	 * #hasReceivedPacket() should be invoked before trying to use this method.
	 *
	 * @return RTPPacket& reference to the packet taken from the queue.
	 *
	 * @warning Behaviour is unknown when trying to perform this operation when incoming packets queue is empty.
	 * @see #hasReceivedPacket()
	 */
	RTPPacket& getReceivedPacket();

	/**
	 *	Local port on which RTP communication is handled
	 */
	int localPortRTP;

	/**
	 *	Local port on which RTCP communication is handled.
	 *
	 *	Local port on which RTCP communication is handled -- it should be <code>localPortRTP + 1</code>.
	 */
	int localPortRTCP;

	/**
	 * Sets remote host IP.
	 *
	 * @param ip std::string representing the IP of the remote host.
	 */
	void setRemoteIP(string ip);

	/**
	 * Sets remote host RTP port.
	 *
	 * @param port integer value representing the RTP port of the remote host.
	 */
	void setRemotePort(int port);

private:
	string remoteIP;
	int remotePort;

	int socketfd_rtp;
	int socketfd_rtcp;
	sockaddr_in addr_in_rtp;
	sockaddr_in addr_in_rtcp;

	bool killThread;
	pthread_t thread;
	Semaphore mutex;

	/**
	 * Sends a RTP packet from the outgoing packets queue using UDP.
	 */
	void sendPacket();

	void recvPacket();

	SynchronizedQueue incomingPackets;
	SynchronizedQueue outgoingPackets;
};

void* RTPAgentSenderThread(void *);
void* RTPAgentReceiverThread(void *);

#endif /* RTPAGENT_H_ */
