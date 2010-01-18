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

/**
 * \~
 * \~polish
 * Klasa opakowująca używane funkcje sieciowe (np. operacje na gniazdach).
 *
 * Zawiera metody statyczne, które sprawdzają wynik otrzymany z funkcji opakowywanej
 * i w razie niepowodzenia rzucają wyjątek @a std::exception lub jego podklasę.
 *
 * \~english
 * Wrapper classes for networking functions.
 *
 * Wrapper classes for networking functions, which check the returned value and
 * throw a @a std::exception or its subclass if the standard function fails.
 *
 * \~
 * @date created 2010-01-17
 * @author: Marek
 * @see std::exception
 */
class Net {
	Net();
public:
	virtual ~Net();
	/**
	 * @see socket(int __domain, int __type, int __protocol)
	 * @throws std::runtime_error
	 */
	static int socket_s(int __domain, int __type, int __protocol);

	/**
	 * @see bind(int socket, const struct sockaddr *address, socklen_t address_len)
	 * @throws BindException
	 */
	static int bind_s(int socket, const struct sockaddr *address,
			socklen_t address_len);
};

#endif /* NET_H_ */
