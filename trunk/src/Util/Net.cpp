/*
 * Net.cpp
 *
 *  Created on: 2010-01-17
 *      Author: marek
 */

#include "Net.h"
#include "../Exceptions/BindException.h"

Net::Net() {
	// TODO Auto-generated constructor stub

}

Net::~Net() {
	// TODO Auto-generated destructor stub
}

int Net::bind_s(int socket, const struct sockaddr *address, socklen_t address_len) {
	int result;
	result = bind(socket, address, address_len);
	if (result == -1) {
		throw BindException(address, address_len);
	}
	return result;
}

int Net::socket_s(int __domain, int __type, int __protocol) {
	int sockfd = socket(__domain, __type, __protocol);
	if (sockfd == -1) {
		throw runtime_error("Can't create UDP socket!");
	}
	return sockfd;
}
