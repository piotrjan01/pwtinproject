/*
 * RTPAgent.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include "DeserializationException.h"
#include "RTPAgent.h"

RTPAgent::RTPAgent() :
	mutex(0) {

	// =========================================================================
	// utwórz gniazdo RTP
	PRN_(1, "Creating socket for RTP...");
	socketfd_rtp = socket(AF_INET, SOCK_DGRAM, 0);
	VAR(socketfd_rtp);
	if (socketfd_rtp == -1) {
		throw runtime_error("Can't create UDP socket!");
	}

	PRN_(1, "Binding to address...");
	sockaddr_in addr_rtp;
	addr_rtp.sin_family = AF_INET;
	addr_rtp.sin_addr.s_addr = htonl(INADDR_ANY);
	addr_rtp.sin_port = 0;

	if (bind(socketfd_rtp, (sockaddr *) &addr_rtp, sizeof(addr_rtp)) == -1) {
		throw runtime_error("Can't bind to address!");
	}

	int length = sizeof(addr_rtp);
	if (getsockname(socketfd_rtp, (sockaddr *) &addr_rtp, (socklen_t *) &length)
			== -1) {
		throw runtime_error("Can't get assigned address!");
	}
	// =========================================================================
	// przygotuj strukturę sockaddr_in (częściowo, reszta przy void setRemotePort, setRemoteAddress)
	addr_in_rtp.sin_family = AF_INET;

	//	 ustaw localPort na port gniazda
	localPort = ntohs(addr_rtp.sin_port);
	VAR_(1, inet_ntoa(addr_rtp.sin_addr));
	VAR_(1, localPort);

	// =========================================================================
	// utwórz gniazdo RTCP
	PRN_(1, "Creating socket for RTCP...");
	VAR(socketfd_rtcp);
	socketfd_rtcp = socket(AF_INET, SOCK_DGRAM, 0);
	if (socketfd_rtcp == -1) {
		throw runtime_error("Can't create UDP socket!");
	}

	PRN_(1, "Binding to address...");
	sockaddr_in addr_rtcp;
	addr_rtcp.sin_family = AF_INET;
	addr_rtcp.sin_addr.s_addr = addr_rtp.sin_addr.s_addr;
	addr_rtcp.sin_port = htons(localPort + 1);

	if (bind(socketfd_rtcp, (sockaddr *) &addr_rtcp, sizeof(addr_rtp)) == -1) {
		throw runtime_error("Can't bind to address!");
	}

	length = sizeof(addr_rtcp);
	if (getsockname(socketfd_rtcp, (sockaddr *) &addr_rtcp,
			(socklen_t *) &length) == -1) {
		throw runtime_error("Can't get assigned address!");
	}

	// =========================================================================
	// przygotuj strukturę sockaddr_in (częściowo, reszta przy void setRemotePort, setRemoteAddress)
	addr_in_rtcp.sin_family = AF_INET;

	//	 ustaw localPort na port gniazda
	localPortRTCP = ntohs(addr_rtcp.sin_port);
	VAR_(1, inet_ntoa(addr_rtcp.sin_addr));
	VAR_(1, localPortRTCP);

	// =========================================================================
	killThread = false;
	pthread_create(&thread, NULL, RTPAgentReceiverThread, (void *) this);
	mutex.V();
}

RTPAgent::~RTPAgent() {
	mutex.P();
	killThread = true;
	mutex.V();

	pthread_join(thread, (void **) NULL);

	PRN_(1, "Closing socket...");
	if (close(socketfd_rtp) != 0)
		throw runtime_error("Can't close socket!");
	PRN_(1, "OK");

}

void RTPAgent::setRemoteIP(string ip) {
	remoteIP = ip;
	addr_in_rtp.sin_addr.s_addr = inet_addr(remoteIP.c_str());
	VAR_(1, remoteIP);
}

void RTPAgent::setRemotePort(int port) {
	remotePort = port;
	addr_in_rtp.sin_port = htons(remotePort);
	VAR_(1, remotePort);
}

void RTPAgent::sendPacket(RTPPacket& rtpPacket) {
	PRN_(2, "RTP: sendPacket");
	ostringstream oss;
	rtpPacket.toStream(oss);
	sendto(socketfd_rtp, oss.str().data(), oss.str().length(), 0,
			(sockaddr *) &addr_in_rtp, sizeof(addr_in_rtp));

	PRN_(2, "-----   RTPPacket sent    -----");
	PRNBITS_(2, oss.str());
	PRN_(2, "--------------------------------");
}

bool RTPAgent::hasReceivedPacket() {
	return !incomingPackets.empty();
}

RTPPacket& RTPAgent::getReceivedPacket() {
	return *incomingPackets.popFront();
}

void RTPAgent::recvPacket() {
	static const int BUF_LENGTH = 32768;
	// TODO
	sockaddr_in sender;
	unsigned int senderlength;
	char buf[BUF_LENGTH];
	if (0 >= recvfrom(socketfd_rtp, buf, 10, MSG_PEEK | MSG_DONTWAIT, NULL,
			NULL)) {
		return;
	}
	int length = recvfrom(socketfd_rtp, buf, BUF_LENGTH, 0,
			(sockaddr *) &sender, &senderlength);

	RTPPacket* rtpPacket;
	try {
		rtpPacket = new RTPPacket(buf, length);
	} catch (DeserializationException& e) {
		PRN("Received invalid data");
		return;
	}
	PRN_(2, "-----  RTPPacket received  -----");
	PRNBITS_(2, rtpPacket->toString());
	PRN_(2, "--------------------------------");
	VAR(length);
	incomingPackets.pushBack(rtpPacket);
}

void* RTPAgentReceiverThread(void* rtpAgent) {
	RTPAgent* agent = (RTPAgent*) rtpAgent;
	PRN_(1, "receiver thread started");
	while (1) {
		sleep(0.1);
		agent->mutex.P();
		agent->recvPacket();
		if (agent->killThread) {
			agent->mutex.V();
			break;
		}
		agent->mutex.V();
	}
	PRN_(1, "receiver thread stopped");
	return NULL;
}
