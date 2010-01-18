/*
 * BindException.cpp
 *
 *  Created on: 2010-01-18
 *      Author: marek
 */

#include "BindException.h"

const char* BindException::DEFAULT_MESSAGE = "Can't bind to address ";

BindException::BindException() throw () {
	oss << DEFAULT_MESSAGE;
	msg = oss.str();
}

BindException& BindException::operator =(const BindException& another) throw () {
	if (this == &another) {
		return *this;
	}
	// TODO
	msg = another.msg;
	return *this;
}

const char *BindException::what() const throw () {
	return msg.c_str();
}

BindException::BindException(const struct sockaddr *address,
		socklen_t address_len) throw () {
	struct sockaddr_in* address_in = (struct sockaddr_in*) address;
	oss << DEFAULT_MESSAGE; // may be changed in future
	oss << inet_ntoa(address_in->sin_addr);
	oss << ":";
	oss << ntohs(address_in->sin_port);
	msg = oss.str();
}

BindException::BindException(const BindException& another) throw () {
	msg = another.msg;
}

BindException::~BindException() throw () {
}
