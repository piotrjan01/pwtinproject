/**
 * @file Net.h
 * @brief Wrapper classes for networking functions.
 *
 * Wrapper classes for networking functions, which check the returned value and
 * throw a \a std::runtime_error if the standard function fails.
 *
 * @date created 2010-01-17
 * @author: Marek
 * @see std::runtime_error
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
