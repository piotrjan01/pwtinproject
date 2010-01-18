/**
 * @file BindException.h
 *
 * @date created 2010-01-18
 * @author Marek Kurdej
 */

#ifndef BINDEXCEPTION_H_
#define BINDEXCEPTION_H_

#include <exception>
#include <sstream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
 * \~
 * \~polish
 * Wyjątek rzucany w razie niepowodzenia funkcji <code>bind</code>
 * \~english
 * Exception thrown in case of failure of function <code>bind</code>
 * \~
 */
class BindException: public std::exception {
	static const char* DEFAULT_MESSAGE;
public:
	BindException() throw ();
			BindException(const struct sockaddr *address, socklen_t address_len) throw ();
	BindException(const BindException& e) throw ();
	BindException& operator=(const BindException& e) throw ();

	virtual ~BindException() throw ();
	virtual const char* what() const throw ();
private:
	std::ostringstream oss;
	std::string msg;
};

#endif /* BINDEXCEPTION_H_ */
