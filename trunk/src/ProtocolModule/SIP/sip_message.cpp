#include <cstdlib>
#include <iostream>
#include <string>

#include "sip_message.h"
#include "sip_authentication.h"
#include "parsing.h"

using namespace std;

bool SIP_Message::isReply() { 
	return (rline.find("SIP/2.0 ") == 0); 
}

bool SIP_Message::isRequest() {
	return !isReply(); 
}

int SIP_Message::replyCode() {
	if (!isReply()) 
		return 0;
	else 
		return atoi( rline.c_str() + 8 );
}

string SIP_Message::method() {
	if (!isRequest()) 
		return "";
	else
		return rline.substr(0, rline.find(" "));
}

string SIP_Message::getField(string name) {
	for (vector<string>::const_iterator iter = lines.begin(); iter != lines.end(); ++iter) 	
		if (getKey(*iter, ':') == name) 
			return trim(getValue(*iter, ':'));
	
	return "";
}

void SIP_Message::setField(string name, string value) {

	for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); ++iter) 	
		if (getKey(*iter, ':') == name) {
			*iter = name + " " + value;
			return;
		}

	lines.push_back( name + " " + value );
}

SIP_Message::SIP_Message( char *buf, int len ) {

	buf[len] = '\000';

	string packet(buf);

	rline = packet.substr( 0, packet.find("\015\012") );
	packet = packet.substr( packet.find("\015\012") + 2 );

	string nextline;

	while (packet.find("\015\012") != string::npos) {
		nextline = packet.substr( 0, packet.find("\015\012") );

		if (nextline.length() == 0) 
			break;

		if (getKey(nextline, ':') == "Via")
			via.push_back(nextline);
		else
			lines.push_back(nextline);

		packet = packet.substr( packet.find("\015\012") + 2 );
	}
}

SIP_Authentication SIP_Message::getAuthentication() {
		
		SIP_Authentication result;
		string s = getField("WWW-Authenticate");
		vector<string> v = split(s, ' ');
		
		if (v.size() != 2)
			return result;

		vector<string> u = split(v[1], ',');

		for (unsigned int i = 0; i < u.size(); ++i) {
			string k = getKey(u[i]);
			string v = trim(getValue(u[i]), '\"');
			if (k == "realm") 
				result.realm = v;
			else if (k == "algorithm") 
				result.algorithm = v;
			else if (k == "nonce")
				result.nonce = v;
		}

		return result;
}

string SIP_Message::toStream() {
	string result = rline + "\015\012";
	
	for (vector<string>::const_iterator iter = via.begin(); iter != via.end(); ++iter)
		result += *iter + "\015\012";

	for (vector<string>::const_iterator iter = lines.begin(); iter != lines.end(); ++iter)
		result += *iter + "\015\012";

return result + "\015\012";
}

std::ostream & operator<<(std::ostream & out, const SIP_Message & m) {

	out << m.rline << endl;

	for (vector<string>::const_iterator iter = m.via.begin(); iter != m.via.end(); ++iter)
		out << *iter << endl;

	for (vector<string>::const_iterator iter = m.lines.begin(); iter != m.lines.end(); ++iter)
		out << *iter << endl;

	return out;
}

