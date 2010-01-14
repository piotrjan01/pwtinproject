#include <string>
#include <sstream>
#include <iomanip>

#include <openssl/md5.h>

#include "sip_authentication.h"

using namespace std;

string md5toString(unsigned char * t) {
	stringstream out;
	out << hex;
	for (int i = 0; i < 16; ++i)
		out << setfill('0') << setw(2) << (int) t[i];
	return out.str();
}

string SIP_Authentication::getMD5() {
	unsigned char h1[16];
	unsigned char h2[16];
	unsigned char result[16];

	string a1 = user + ":" + realm + ":" + pass;
	string a2 = method + ":" + uri;

	MD5( (unsigned char *) a1.c_str(), (unsigned long) a1.length(), h1 );
	MD5( (unsigned char *) a2.c_str(), (unsigned long) a2.length(), h2 );

	string a3 = md5toString(h1) + ":" + nonce + ":" + md5toString(h2);

	MD5( (unsigned char *) a3.c_str(), (unsigned long) a3.length(), result );

	return md5toString( result );
}
