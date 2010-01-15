// rand()
#include <cstdlib>
// std:ostream
#include <iostream>
// throw runtime_error...
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
#include "sip_agent.h"
#include "parsing.h"

using namespace std;

void * SIP_AgentThread( void * sip_agent );

SIP_Agent::SIP_Agent(string localaddr) : mutex(0), wait(0) {

	cout << "Creating socket...";
	sock = socket( AF_INET, SOCK_DGRAM, 0);
	
	if (sock == -1)
		throw runtime_error("Can't create UDP socket!");

	cout << "OK, assigned socket number is " << sock << endl;

	cout << "Binding to address...";
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(localaddr.c_str());  // INADDR_ANY;
	address.sin_port = 0;
	
	if (bind(sock, (sockaddr *) &address, sizeof(address)) == -1)
		throw runtime_error("Can't bind to address!");

	cout << "OK, address: ";

	int length = sizeof( address );
	if (getsockname( sock, (sockaddr *) &address, (socklen_t *) &length) == -1) 
		throw runtime_error("Can't get assigned address!");

	cout << inet_ntoa( address.sin_addr ) << ":" << ntohs( address.sin_port ) << endl;

	killThread = false;
	pthread_create(&thread, NULL, SIP_AgentThread, (void *) this );
	mutex.V();

}

SIP_Agent::~SIP_Agent() {

	mutex.P();
	killThread = true;
	mutex.V();

	pthread_join( thread, (void **)NULL );

	cout << "Closing socket...";
	if (close(sock) != 0) 
		throw runtime_error("Can't close socket!");
	cout << "OK" << endl;	
}

string SIP_Agent::generateCallID() {
	string callid = "";

	for (int i = 0; i < 10; ++i) 
		callid += toString( rand() % 10 );

	return callid + "@" + proxy;
}

void SIP_Agent::generateFromTag() {
	tag = "";

	for (int i = 0; i < 10; ++i) {
		int v = rand() % 16;
		if (v < 10) 
			tag += toString( v );
		else 
			tag += toString( (char) (((int) 'a') + (v - 10)) );
	}

}

string SIP_Agent::generateBranch() {
	string result = "";
	for (int i = 0; i < 20; ++i) 
		result += toString( rand() % 10 );
	return result;
}

string SIP_Agent::addressString() {
	return string(inet_ntoa( address.sin_addr )) + ":" + toString(ntohs( address.sin_port ));
}

void SIP_Agent::sendMessage(SIP_Message &m, string address, string port) {
	string s = m.toStream();

	sockaddr_in r;

	r.sin_family = AF_INET;
	r.sin_addr.s_addr = inet_addr(address.c_str());
	r.sin_port = htons( atoi(port.c_str()) );

	sendto(sock, s.c_str(), s.length(), 0, (sockaddr *) &r, sizeof( r ));

	cout << "-----   SIP Message sent   -----" << endl;
	cout << m;
	cout << "--------------------------------" << endl;
}

void SIP_Agent::replyToOptions(SIP_Message &m) {
	
	SIP_Message reply;

	reply.rline = "SIP/2.0 200 OK";
	reply.via.push_back(m.via[0]);

	reply.lines.push_back("Max-Forwards: 70");
	
	reply.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	reply.lines.push_back("From: " +  m.getField("From"));
	reply.lines.push_back("To: " + m.getField("To"));

	reply.lines.push_back("Call-ID: " + m.getField("Call-ID"));
	reply.lines.push_back("CSeq: " + m.getField("CSeq"));
	
	reply.lines.push_back("Accept: application/sdp");
	reply.lines.push_back("Accept-Language: en");

//	m.lines.push_back("Allow: INVITE, ACK, CANCEL, OPTIONS, BYE, REFER, NOTIFY, MESSAGE, SUBSCRIBE, INFO");

	reply.lines.push_back("Allow: INVITE, ACK, OPTIONS, BYE");

	reply.lines.push_back("Content-Length: 0");

	sendMessage(reply, proxy, "8060");

}

void SIP_Agent::replyAck(SIP_Message &m, std::string information) {
	
	SIP_Message reply;

	reply.rline = "ACK " + information + " SIP/2.0";
	reply.via.push_back(m.via[0]);

	reply.lines.push_back("Max-Forwards: 70");
	
	reply.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	reply.lines.push_back("From: " +  m.getField("From"));
	reply.lines.push_back("To: " + m.getField("To"));

	reply.lines.push_back("Call-ID: " + m.getCallID());
	reply.lines.push_back("CSeq: 1 ACK");	

	reply.lines.push_back("Content-Length: 0");

	sendMessage(reply, proxy, "8060");

}

void SIP_Agent::receiveMessage() { 

	sockaddr_in sender;
	unsigned int senderlength;

	char buf[32768];

	if (0 >= recvfrom(sock, buf, 10, MSG_PEEK | MSG_DONTWAIT, NULL, NULL))
		return;

	int buflength = recvfrom(sock, buf, 32768, 0, (sockaddr *) &sender, &senderlength);

	SIP_Message m( buf, buflength );

	cout << "----- SIP Message received -----" << endl;
	cout << m;
	cout << "------------ Action ------------" << endl;
	
	if (m.isReply()) {
		cout << "It's a reply to request " + m.getField("CSeq") + " sent by ";
		
		if (m.getField("From") == fromtoline) {
			cout << "this agent." << endl;
			int code = m.replyCode();
			cout << "Result code is " << code << ", ";
			if (code < 100) 
				cout << "that's an error." << endl << "No action." << endl;
			else if (code < 200) 
				cout << "that's OK, let's wait for next messages." << endl << "Waiting for more." << endl;
			else if (code == 200) {
				
				if (m.getCallID() == invite.callid) {
					cout << "that's great, connection established!" << endl ;	

					cout << "Remote address: " << m.getSdpAddress() << ":" << m.getSdpPort() << endl;

					remoteRtpPort = atoi( m.getSdpPort().c_str() );
					remoteRtpAddress = m.getSdpAddress();

					replyAck(m);

					if (waitingfor == CALL) {
						waitingfor = NONE;
						wait.V();
					}


				} else if (m.getCallID() == registration.callid) {
					cout << "that's great, we're registered!" << endl;					

					if (waitingfor == REGISTER) {
						waitingfor = NONE;
						wait.V();
					}
				}

			} else if (code < 300) {
			
			} else if (code < 400) {
			
			} else if (code == 401) {
				cout << "the server wants authentication." << endl;
				SIP_Authentication auth = m.getAuthentication();
				if (auth.algorithm != "MD5") {
					cout << "Authentication with " << auth.algorithm << " is not implemented, I give up." << endl;
					cout << "No action" << endl;
					waitingfor = NONE;
					wait.V();
				} else {
					cout << "Register again using authentication." << endl;
					auth.user = user;
					auth.pass = pass;
					auth.method = "REGISTER";
					auth.uri = "sip:" + proxy;
					registration.authentication = auth;
					Register();
				}
				
			} else if (code == 403) {
				
				if (waitingfor == REGISTER) {
					cout << "Can't register, wrong password?" << endl;
					waitingfor = NONE;
					wait.V();
				}

			} else if (code == 404) {
				
				if (waitingfor == CALL) {
					cout << "Can't connect, user not found." << endl;
					waitingfor = NONE;
					wait.V();
				}

			} else if (code == 407) {
				
				cout << "proxy authentication is required" << endl;
				replyAck(m);

				SIP_Authentication auth = m.getProxyAuthentication();
				if (auth.algorithm != "MD5") {
					cout << "Authentication with " << auth.algorithm << " is not implemented, I give up." << endl;
					cout << "No action" << endl;
					waitingfor = NONE;
					wait.V();
				} else {
					cout << "Invite again using authentication." << endl;
					auth.user = user;
					auth.pass = pass;
					auth.method = "INVITE";
					auth.uri = "sip:" + partnerID + "@" + proxy;
					registration.authentication = auth;
					Call();
				}

			} else if (code == 480) {
				
				cout << "temporary unavailable, busy? rejected?" << endl;
				if (waitingfor == CALL) {
					cout << "Can't connect, user not found." << endl;
					waitingfor = NONE;
					wait.V();
				}

			}
		} else {
			cout << m.getField("From") << "." << endl;
			cout << "No action." << endl;
		}

	} else {
		cout << "It's a request - ";
		cout << m.getField("From") << " requests " << m.method() << " from " << m.getField("To") << endl;
		if (m.method() == "OPTIONS") 
			replyToOptions(m);
	}

	cout << "--------------------------------" << endl;

}


void SIP_Agent::Register() {
	SIP_Message m;
	m.rline = "REGISTER sip:" + proxy + " SIP/2.0";
	m.via.push_back("Via: SIP/2.0/UDP " + addressString() + ";branch=" + branch);

	m.lines.push_back("Max-Forwards: 70");
	
	m.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	fromtoline = "\"" + user + "\"<sip:" + user + "@" + proxy + ">;tag=" + tag;

	m.lines.push_back("From: " + fromtoline);
	m.lines.push_back("To: \"" + user + "\"<sip:" + user + "@" + proxy + ">");

	m.lines.push_back("Call-ID: " + registration.callid);
	m.lines.push_back("CSeq: " + toString(++registration.cseq) + " REGISTER");
	m.lines.push_back("Expires: 3600");

	if (registration.authentication.algorithm == "MD5") {
		m.lines.push_back("Proxy-Authorization: Digest username=\"" + registration.authentication.user +
				"\",realm=\"" + registration.authentication.realm + "\",nonce=\"" + registration.authentication.nonce +
				"\",uri=\"" + registration.authentication.uri + "\",response=\"" + registration.authentication.getMD5() +
				"\",algorithm=MD5");
	}

	m.lines.push_back("Content-Length: 0");

	sendMessage(m, proxy, "8060");
}

void SIP_Agent::Call() {
	
	SIP_Message	m;

	m.rline = "INVITE sip:" + partnerID + "@" + proxy + " SIP/2.0";
	m.via.push_back("Via: SIP/2.0/UDP " + addressString() + ";branch=" + branch);

	m.lines.push_back("Max-Forwards: 70");
	
	m.lines.push_back("Contact: <sip:" + user + "@" + addressString() + ">");

	m.lines.push_back("From: " + fromtoline);
	m.lines.push_back("To: \"" + partnerID + "\"<sip:" + partnerID + "@" + proxy + ">");

	m.lines.push_back("Call-ID: " + invite.callid);
	m.lines.push_back("CSeq: " + toString(invite.cseq) + " INVITE");

	m.lines.push_back("Allow: INVITE, ACK, OPTIONS, BYE");

	if (registration.authentication.algorithm == "MD5") {
		m.lines.push_back("Authorization: Digest username=\"" + registration.authentication.user +
				"\",realm=\"" + registration.authentication.realm + "\",nonce=\"" + registration.authentication.nonce +
				"\",uri=\"" + registration.authentication.uri + "\",response=\"" + registration.authentication.getMD5() +
				"\",algorithm=MD5");
	}

	m.lines.push_back("Content-Type: application/sdp");

	m.body = "v=0\n";
	m.body += string("o=- 9 2 IN IP4 ") + inet_ntoa( address.sin_addr ) + string("\n");
	m.body += "s=CounterPath X-Lite 3.0\n";
	m.body += string("c=IN IP4 ") + inet_ntoa( address.sin_addr ) + string("\n");
	m.body += string("t=0 0\n");
	m.body += string("m=audio ") + toString(localRtpPort) + string(" RTP/AVP 107 101\n");
	m.body += string("a=alt:1 1 : JelDk9w1 QTsOTJSj ") + inet_ntoa( address.sin_addr ) + " " + toString(localRtpPort) + "\n";
	m.body += "a=fmtp:101 0-15\n";
	m.body += "a=rtpmap:107 BV32/16000\n";
	m.body += "a=rtpmap:101 telephone-event/8000\n";
	m.body += "a=sendrecv\n";

	m.lines.push_back("Content-Length: " + toString(m.body.length()));

	sendMessage(m, proxy, "8060");

}

void SIP_Agent::Call(std::string id, unsigned short port) {
	
	partnerID = id;
	localRtpPort = port;

	invite.cseq = 1;
	invite.callid = generateCallID();
	invite.authentication.algorithm = "";
	
	waitingfor = CALL;

	Call();

	wait.P();
}


void SIP_Agent::Register(string user, string pass, string proxy) {

	mutex.P();

	this->user  = user;
	this->pass  = pass;
	this->proxy = proxy;

	generateFromTag();
	
	registration.authentication.algorithm = "";
	registration.cseq = 0;
	registration.callid = generateCallID();
	branch = generateBranch();

	Register();

	waitingfor = REGISTER;

	mutex.V();

	wait.P();
}

void * SIP_AgentThread( void * sip_agent ) {

	SIP_Agent * agent = (SIP_Agent *) sip_agent;

	cout << "SIP AGENT thread started." << endl;

	while (1) {

		sleep(0.1);
		agent->mutex.P();
		
		agent->receiveMessage();

		if (agent->killThread)
			break;

		agent->mutex.V();
	}

	agent->mutex.V();

	return NULL;

}

