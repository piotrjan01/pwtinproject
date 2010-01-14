/* 
 * sip_message.h
 * Reprezentacja wiadomosci SIP.
 */

#ifndef SIP_MESSAGE_H
#define SIP_MESSAGE_H

#include <iostream>
#include <string>
#include <vector>

#include "sip_authentication.h"

struct SIP_Message {
	
	friend std::ostream & operator<<(std::ostream & out, const SIP_Message & m);

	std::string				 rline;
	std::vector<std::string> via;
	std::vector<std::string> lines;

	SIP_Message() {}
	SIP_Message( char *buf, int len );

	std::string getField(std::string name);
	void setField(std::string name, std::string value);

	bool isReply();
	bool isRequest();

	int replyCode();
	std::string method();

	SIP_Authentication getAuthentication();
	std::string toStream();

};

#endif

