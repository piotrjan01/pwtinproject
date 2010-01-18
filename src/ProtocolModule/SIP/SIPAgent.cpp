// rand()
#include <cstdlib>
// std:ostream
#include <iostream>
// throw std::runtime_error...
#include <stdexcept>

// Sockety
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Watki
#include <pthread.h>

#include <string>
#include <sstream>
#include <vector>

#include "semaphore.h"
#include "SIPAgent.h"
#include "parsing.h"

void * SIPAgentThread(void * SIPAgent);

SIPAgent::SIPAgent(std::string localaddr) :
	mutex(0), wait(0) {

	registered = false;
	connectionEstablished = false;
	sleepTime = 1.0;

	std::cout << "Creating socket...";
	sock = Net::socket_s(AF_INET, SOCK_DGRAM, 0);
	std::cout << "OK, assigned socket number is " << sock << std::endl;

	std::cout << "Binding to address...";
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(localaddr.c_str()); // INADDR_ANY;
	address.sin_port = 0;
	Net::bind_s(sock, (sockaddr *) &address, sizeof(address));
	std::cout << "OK, address: ";

	int length = sizeof(address);
	if (getsockname(sock, (sockaddr *) &address, (socklen_t *) &length) == -1)
		throw std::runtime_error("Can't get assigned address!");

	std::cout << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port)
			<< std::endl;

	killThread = false;
	pthread_create(&thread, NULL, SIPAgentThread, (void *) this);
	mutex.V();

}

SIPAgent::~SIPAgent() {

	mutex.P();
	killThread = true;
	mutex.V();

	pthread_join(thread, (void **) NULL);

	std::cout << "Closing socket...";
	if (close(sock) != 0)
		throw std::runtime_error("Can't close socket!");
	std::cout << "OK" << std::endl;
}

std::string SIPAgent::generateCallID() {
	std::string callid = "";

	for (int i = 0; i < 10; ++i)
		callid += toString(rand() % 10);

	return callid + "@" + proxy;
}

void SIPAgent::generateFromTag() {
	tag = "";

	for (int i = 0; i < 10; ++i) {
		int v = rand() % 16;
		if (v < 10)
			tag += toString(v);
		else
			tag += toString((char) (((int) 'a') + (v - 10)));
	}

}

std::string SIPAgent::generateBranch() {
	std::string result = "";
	for (int i = 0; i < 20; ++i)
		result += toString(rand() % 10);
	return result;
}

std::string SIPAgent::addressString() {
	return std::string(inet_ntoa(address.sin_addr)) + ":" + toString(ntohs(
			address.sin_port));
}

void SIPAgent::sendMessage(SIPMessage &m, std::string address,
		unsigned short port) {
	std::string s = m.toStream();

	sockaddr_in r;

	r.sin_family = AF_INET;
	r.sin_addr.s_addr = inet_addr(address.c_str());
	r.sin_port = htons(port);

	sendto(sock, s.c_str(), s.length(), 0, (sockaddr *) &r, sizeof(r));

	/*	std::cout << "-----   SIP Message sent   -----" << std::endl;
	 std::cout << m;
	 std::cout << "--------------------------------" << std::endl;*/
	std::cout << "SIP: >> " << m.rline << std::endl;
}

void SIPAgent::replyToOptions(SIPMessage &m) {

	SIPMessage reply;

	reply.rline = "SIP/2.0 200 OK";
	reply.via.push_back(m.via[0]);

	reply.lines.push_back("Max-Forwards: 70");

	reply.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	reply.lines.push_back("From: " + m.getField("From"));
	reply.lines.push_back("To: " + m.getField("To"));

	reply.lines.push_back("Call-ID: " + m.getField("Call-ID"));
	reply.lines.push_back("CSeq: " + m.getField("CSeq"));

	reply.lines.push_back("Accept: application/sdp");
	reply.lines.push_back("Accept-Language: en");

	reply.lines.push_back("Allow: INVITE, ACK, OPTIONS, BYE");

	reply.lines.push_back("Content-Length: 0");

	sendMessage(reply, proxy, proxyPort);

}

void SIPAgent::replyAck(SIPMessage &m, std::string information) {

	SIPMessage reply;

	reply.rline = "ACK " + information + " SIP/2.0";
	reply.via.push_back(m.via[0]);

	reply.lines.push_back("Max-Forwards: 70");

	reply.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	reply.lines.push_back("From: " + m.getField("From"));
	reply.lines.push_back("To: " + m.getField("To"));

	reply.lines.push_back("Call-ID: " + m.getCallID());
	reply.lines.push_back("CSeq: 1 ACK");

	reply.lines.push_back("Content-Length: 0");

	sendMessage(reply, proxy, proxyPort);

}

void SIPAgent::replyRinging(SIPMessage &m) {

	SIPMessage reply;

	reply.rline = "SIP/2.0 180 Ringing";
	reply.via = m.via;

	reply.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	reply.lines.push_back("From: " + m.getField("From"));
	reply.lines.push_back("To: " + m.getField("To") + ";tag=" + tag);

	reply.lines.push_back("Call-ID: " + m.getCallID());
	reply.lines.push_back("CSeq: " + m.getField("CSeq"));

	reply.lines.push_back("Content-Length: 0");

	sendMessage(reply, proxy, proxyPort);

}

void SIPAgent::answerCall(SIPMessage &m) {

	SIPMessage reply;

	reply.rline = "SIP/2.0 200 OK";
	reply.via = m.via;

	reply.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	reply.lines.push_back("From: " + m.getField("From"));
	reply.lines.push_back("To: " + m.getField("To") + ";tag=" + tag);

	invite.callid = m.getCallID();
	invite.cseq = atoi(m.getField("CSeq").c_str());
	partnerID = m.getField("From");
	int pb = partnerID.find("sip:") + 4;
	int pe = partnerID.find("@", pb);
	partnerID = partnerID.substr(pb, pe - pb);

	reply.lines.push_back("Call-ID: " + m.getCallID());
	reply.lines.push_back("CSeq: " + m.getField("CSeq"));

	reply.lines.push_back("Record-Route: " + m.getField("Record-Route"));

	reply.lines.push_back("Allow: INVITE, ACK, OPTIONS, BYE");

	reply.lines.push_back("Content-Type: application/sdp");

	reply.body = "v=0\n";
	reply.body += std::string("o=- 9 2 IN IP4 ") + inet_ntoa(address.sin_addr)
			+ std::string("\n");
	reply.body += "s=X-Lite\n";
	//    reply.body += "s=CounterPath X-Lite 3.0\n";
	reply.body += std::string("c=IN IP4 ") + inet_ntoa(address.sin_addr)
			+ std::string("\n");
	reply.body += std::string("t=0 0\n");
	reply.body += std::string("m=audio ") + toString(localRtpPort)
			+ std::string(" RTP/AVP 0 8 3 98 97 101 107\n");
	//    reply.body += std::string("m=audio ") + toString(localRtpPort) + std::string(" RTP/AVP 107 101\n");
	reply.body += std::string("a=alt:1 1 : JelDk9w1 QTsOTJSj ") + inet_ntoa(
			address.sin_addr) + " " + toString(localRtpPort) + "\n";
	reply.body += "a=rtpmap:0 pcmu/8000\n";
	reply.body += "a=rtpmap:8 pcma/8000\n";
	reply.body += "a=rtpmap:3 gsm/8000\n";
	reply.body += "a=rtpmap:98 iLBC/8000\n";
	reply.body += "a=rtpmap:97 speex/8000\n";
	reply.body += "a=rtpmap:101 telephone-event/8000\n";
	reply.body += "a=rtpmap:107 BV32/16000\n";
	reply.body += "a=fmtp:101 0-15\n";
	reply.body += "a=sendrecv\n";

	reply.lines.push_back("Content-Length: " + toString(reply.body.length()));

	sendMessage(reply, proxy, proxyPort);

}

void SIPAgent::receiveMessage() {

	sockaddr_in sender;
	unsigned int senderlength;

	char buf[32768];

	if (0 >= recvfrom(sock, buf, 10, MSG_PEEK | MSG_DONTWAIT, NULL, NULL))
		return;

	int buflength = recvfrom(sock, buf, 32768, 0, (sockaddr *) &sender,
			&senderlength);

	SIPMessage m(buf, buflength);

	/*	std::cout << "----- SIP Message received -----" << std::endl;
	 std::cout << m;
	 std::cout << "------------ Action ------------" << std::endl;*/

	std::cout << "SIP: << " << m.rline << std::endl;

	if (m.isReply()) {
		//std::cout << "It's a reply to request " + m.getField("CSeq") + " sent by ";

		if (m.getField("From") == fromtoline) {
			//std::cout << "this agent." << std::endl;
			int code = m.replyCode();
			//std::cout << "Result code is " << code << ", ";
			if (code < 100)
				std::cout << "SIP: Error" << std::endl;
			else if (code == 200) {

				if (m.getCallID() == invite.callid) {
					if (waitingfor == CALL) {

						//std::cout << "that's great, connection established!" << std::endl ;

						std::cout << "SIP: Connection established."
								<< std::endl;
						std::cout << "SIP: Remote RTP address: "
								<< m.getSdpAddress() << ":" << m.getSdpPort()
								<< std::endl;

						remoteRtpPort = atoi(m.getSdpPort().c_str());
						remoteRtpAddress = m.getSdpAddress();
						connectionEstablished = true;

						partnerTag = m.getField("To");
						partnerTag = partnerTag.substr(partnerTag.find("tag=")
								+ 4);

						replyAck(m);

						waitingfor = NONE;
						wait.V();
					}

				} else if (m.getCallID() == registration.callid) {
					if (unregistering)
						std::cout << "SIP: Unregistered." << std::endl;
					else
						std::cout << "SIP: Registered." << std::endl;

					if (waitingfor == REGISTER) {
						registered = true;
						waitingfor = NONE;
						wait.V();
					}
				}

			} else if (code < 300) {

			} else if (code < 400) {

			} else if (code == 401) {
				// std::cout << "the server wants authentication." << std::endl;
				SIPAuthentication auth = m.getAuthentication();
				if (auth.algorithm != "MD5") {
					std::cout << "SIP: Authentication with " << auth.algorithm
							<< " is not implemented, I give up." << std::endl;
					// std::cout << "No action" << std::endl;
					registered = false;
					waitingfor = NONE;
					wait.V();
				} else {
					std::cout << "SIP: Register again using authentication."
							<< std::endl;
					auth.user = user;
					auth.pass = pass;
					auth.method = "REGISTER";
					auth.uri = "sip:" + proxy;
					registration.authentication = auth;
					Register();
				}

			} else if (code == 403) {

				if (waitingfor == REGISTER) {
					std::cout << "SIP: Can't register, wrong password?"
							<< std::endl;
					registered = false;
					waitingfor = NONE;
					wait.V();
				}

			} else if (code == 404) {

				if (waitingfor == CALL) {
					std::cout << "SIP: Can't connect, user not found."
							<< std::endl;
					waitingfor = NONE;
					wait.V();
				}

			} else if (code == 407) {

				//std::cout << "SIP: Proxy authentication is required." << std::endl;
				replyAck(m);

				SIPAuthentication auth = m.getProxyAuthentication();
				if (auth.algorithm != "MD5") {
					std::cout << "SIP: Authentication with " << auth.algorithm
							<< " is not implemented, I give up." << std::endl;
					waitingfor = NONE;
					wait.V();
				} else {
					std::cout << "SIP: Invite again using authentication."
							<< std::endl;
					auth.user = user;
					auth.pass = pass;
					auth.method = "INVITE";
					auth.uri = "sip:" + partnerID + "@" + proxy;
					registration.authentication = auth;
					Call();
				}

			} else if (code == 480) {

				//std::cout << "SIP: Temporary unavailable, busy? rejected?" << std::endl;
				if (waitingfor == CALL) {
					std::cout << "SIP: Temporary unavailable, busy? rejected?"
							<< std::endl;
					waitingfor = NONE;
					wait.V();
				}

			}
		} else {
			//	std::cout << m.getField("From") << "." << std::endl;
			//std::cout << "No action." << std::endl;
		}

	} else {
		//std::cout << "It's a request - ";
		//std::cout << m.getField("From") << " requests " << m.method() << " from " << m.getField("To") << std::endl;

		if (m.method() == "OPTIONS")
			replyToOptions(m);
		else if (m.method() == "INVITE") {

			std::cout << "SIP: Incoming call. " << std::endl;
			replyRinging(m);

			if (waitingfor == ANSWER) {
				remoteRtpPort = atoi(m.getSdpPort().c_str());
				remoteRtpAddress = m.getSdpAddress();
				answerCall(m);
			}
		} else if (m.method() == "BYE") {

			if (connectionEstablished && (invite.callid == m.getCallID())) {
				std::cout << "SIP: Disconnecting." << std::endl;
				connectionEstablished = false;
			}

		} else if (m.method() == "ACK") {

			if (waitingfor == ANSWER) {
				if (invite.callid == m.getCallID()) {
					std::cout << "SIP: Connection established" << std::endl;
					partnerTag = m.getField("From");
					partnerTag = partnerTag.substr(partnerTag.find("tag=") + 4);
					connectionEstablished = true;
					waitingfor = NONE;
					wait.V();
				}
			}

		}
	}

	//std::cout << "--------------------------------" << std::endl;

}

void SIPAgent::Register() {
	SIPMessage m;
	m.rline = "REGISTER sip:" + proxy + " SIP/2.0";
	m.via.push_back("Via: SIP/2.0/UDP " + addressString() + ";branch=" + branch);

	m.lines.push_back("Max-Forwards: 70");

	m.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	fromtoline = "\"" + user + "\"<sip:" + user + "@" + proxy + ">;tag=" + tag;

	m.lines.push_back("From: " + fromtoline);
	m.lines.push_back("To: \"" + user + "\"<sip:" + user + "@" + proxy + ">");

	m.lines.push_back("Call-ID: " + registration.callid);
	m.lines.push_back("CSeq: " + toString(++registration.cseq) + " REGISTER");

	if (unregistering)
		m.lines.push_back("Expires: 0");
	else
		m.lines.push_back("Expires: 3600");

	if (registration.authentication.algorithm == "MD5") {
		m.lines.push_back("Proxy-Authorization: Digest username=\""
				+ registration.authentication.user + "\",realm=\""
				+ registration.authentication.realm + "\",nonce=\""
				+ registration.authentication.nonce + "\",uri=\""
				+ registration.authentication.uri + "\",response=\""
				+ registration.authentication.getMD5() + "\",algorithm=MD5");
	}

	m.lines.push_back("Content-Length: 0");

	sendMessage(m, proxy, proxyPort);
}

void SIPAgent::Call() {

	SIPMessage m;

	m.rline = "INVITE sip:" + partnerID + "@" + proxy + " SIP/2.0";
	m.via.push_back("Via: SIP/2.0/UDP " + addressString() + ";branch=" + branch);

	m.lines.push_back("Max-Forwards: 70");

	m.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	m.lines.push_back("From: " + fromtoline);
	m.lines.push_back("To: \"" + partnerID + "\"<sip:" + partnerID + "@"
			+ proxy + ">");

	m.lines.push_back("Call-ID: " + invite.callid);
	m.lines.push_back("CSeq: " + toString(invite.cseq) + " INVITE");

	m.lines.push_back("Allow: INVITE, ACK, OPTIONS, BYE");

	if (registration.authentication.algorithm == "MD5") {
		m.lines.push_back("Authorization: Digest username=\""
				+ registration.authentication.user + "\",realm=\""
				+ registration.authentication.realm + "\",nonce=\""
				+ registration.authentication.nonce + "\",uri=\""
				+ registration.authentication.uri + "\",response=\""
				+ registration.authentication.getMD5() + "\",algorithm=MD5");
	}

	m.lines.push_back("Content-Type: application/sdp");

	m.body = "v=0\n";
	m.body += std::string("o=- 9 2 IN IP4 ") + inet_ntoa(address.sin_addr)
			+ std::string("\n");
	m.body += "s=X-Lite\n";
	m.body += std::string("c=IN IP4 ") + inet_ntoa(address.sin_addr)
			+ std::string("\n");
	m.body += std::string("t=0 0\n");
	m.body += std::string("m=audio ") + toString(localRtpPort) + std::string(
			" RTP/AVP 0 8 3 98 97 101 107\n");
	//    m.body += std::string("m=audio ") + toString(localRtpPort) + std::string(" RTP/AVP 107 101\n");
	m.body += std::string("a=alt:1 1 : JelDk9w1 QTsOTJSj ") + inet_ntoa(
			address.sin_addr) + " " + toString(localRtpPort) + "\n";
	m.body += "a=rtpmap:0 pcmu/8000\n";
	m.body += "a=rtpmap:8 pcma/8000\n";
	m.body += "a=rtpmap:3 gsm/8000\n";
	m.body += "a=rtpmap:98 iLBC/8000\n";
	m.body += "a=rtpmap:97 speex/8000\n";
	m.body += "a=rtpmap:101 telephone-event/8000\n";
	m.body += "a=rtpmap:107 BV32/16000\n";
	m.body += "a=fmtp:101 0-15\n";
	m.body += "a=sendrecv\n";

	m.lines.push_back("Content-Length: " + toString(m.body.length()));

	sendMessage(m, proxy, proxyPort);

}

CallInfo SIPAgent::Call(std::string id, unsigned short port) {

	CallInfo result;

	if (!connected()) {
		mutex.P();

		std::cout << "SIP: Calling " << id << std::endl;

		remoteRtpAddress = "0.0.0.0";
		remoteRtpPort = 0;

		partnerID = id;
		localRtpPort = port;

		invite.cseq = 1;
		invite.callid = generateCallID();
		invite.authentication.algorithm = "";

		waitTime = 0;
		waitingfor = CALL;

		Call();

		mutex.V();

		wait.P();
	}

	mutex.P();

	result.remoteIP = remoteRtpAddress;
	result.remotePort = remoteRtpPort;
	result.callID = id;

	mutex.V();

	return result;
}

CallInfo SIPAgent::Answer(unsigned short port) {

	CallInfo result;

	if (!connected()) {
		mutex.P();

		std::cout << "SIP: Waiting for call." << std::endl;

		remoteRtpAddress = "0.0.0.0";
		remoteRtpPort = 0;

		localRtpPort = port;

		waitTime = 0;
		waitingfor = ANSWER;

		mutex.V();

		wait.P();
	}

	mutex.P();

	result.remoteIP = remoteRtpAddress;
	result.remotePort = remoteRtpPort;
	result.callID = partnerID;

	mutex.V();

	return result;
}

void SIPAgent::Disconnect() {

	if (!connected())
		return;

	mutex.P();

	SIPMessage m;

	std::cout << "SIP: Disconnecting." << std::endl;

	m.rline = "BYE sip:" + user + "@" + addressString() + " SIP/2.0";
	m.via.push_back("Via: SIP/2.0/UDP " + addressString() + ";branch=" + branch);

	m.lines.push_back("Max-Forwards: 70");

	m.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	m.lines.push_back("From: " + fromtoline);
	m.lines.push_back("To: \"" + partnerID + "\"<sip:" + partnerID + "@"
			+ proxy + ">;tag=" + partnerTag);

	m.lines.push_back("Call-ID: " + invite.callid);
	m.lines.push_back("CSeq: " + toString(++invite.cseq) + " BYE");

	m.lines.push_back("Content-Length: 0");
	sendMessage(m, proxy, proxyPort);

	mutex.V();
}

void SIPAgent::Unregister() {

	mutex.P();

	if (!registered) {
		mutex.V();
		return;
	}

	unregistering = true;
	registered = false;
	waitingfor = REGISTER;
	waitTime = 0;

	Register();

	mutex.V();

	wait.P();

}

bool SIPAgent::Register(std::string user, std::string pass, std::string proxy,
		unsigned short proxyPort) {

	bool result;

	mutex.P();

	if (registered) {
		mutex.V();
		return true;
	}

	unregistering = false;

	std::cout << "SIP: Registering." << std::endl;

	this->proxyPort = proxyPort;

	connectionEstablished = false;
	this->user = user;
	this->pass = pass;
	this->proxy = proxy;

	generateFromTag();

	registration.authentication.algorithm = "";
	registration.cseq = 0;
	registration.callid = generateCallID();
	branch = generateBranch();

	waitTime = 0;
	waitingfor = REGISTER;

	Register();

	mutex.V();

	wait.P();

	mutex.P();
	result = registered;
	mutex.V();

	return result;
}

bool SIPAgent::connected() {

	bool result;

	mutex.P();

	result = connectionEstablished;

	mutex.V();

	return result;
}

void* SIPAgentThread(void* sipAgent) {
	SIPAgent* agent = (SIPAgent*) sipAgent;

	PRN_(1, "SIP: Agent thread started.");

	agent->mutex.P();

	double sleepTime = agent->sleepTime;

	agent->mutex.V();

	while (1) {

		sleep(sleepTime);
		agent->mutex.P();

		agent->receiveMessage();

		if (agent->killThread)
			break;

		if (agent->waitingfor != NONE) {
			++agent->waitTime;
			double waitSeconds = ((double) agent->waitTime) * sleepTime;
			bool timeout;

			switch (agent->waitingfor) {
			case CALL:
				timeout = (waitSeconds >= 30.0);
				break;
			case ANSWER:
				timeout = (waitSeconds >= 120.0);
				break;
			case REGISTER:
				timeout = (waitSeconds >= 10.0);
				break;
			default:
				timeout = false;
			}

			if (timeout) {
				std::cout << "SIP: Timeout." << std::endl;
				agent->waitingfor = NONE;
				agent->wait.V();
			}

		}

		agent->mutex.V();
	}

	agent->mutex.V();
	PRN_(1, "SIP: Agent thread stopped.");
	return NULL;

}

