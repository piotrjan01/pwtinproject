/*
 * Net.h
 *
 *  Created on: 2010-01-17
 *      Author: marek
 */

#ifndef NET_H_
#define NET_H_

#include <stdexcept>

#include <sys/socket.h>

using namespace std;

class Net {
	Net();
public:
	virtual ~Net();
	static int socket_s(int __domain, int __type, int __protocol);
	static int bind_s(int socket, const struct sockaddr *address, socklen_t address_len);
};

#endif /* NET_H_ */
