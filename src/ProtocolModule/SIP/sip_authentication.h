/* 
 * sip_authentication.h
 * Uwierzytelnienie w sesjach SIP.
 */

#ifndef SIP_AUTHENTICATION_H
#define SIP_AUTHENTICATION_H

#include <string>

struct SIP_Authentication {

	std::string realm, nonce, algorithm;
	std::string uri, method;
	std::string user, pass;

	std::string getMD5();

};

#endif
