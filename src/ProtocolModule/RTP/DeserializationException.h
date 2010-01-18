/**
 * @file DeserializationException.h
 *
 *
 * @date created 2010-01-16
 * @author: Marek
 */

#ifndef DESERIALIZATIONEXCEPTION_H_
#define DESERIALIZATIONEXCEPTION_H_

#include <exception>

using namespace std;

/**
 * Exception thrown by RTPPacket constructor when object deserialization from data fails.
 *
 * Exception thrown by RTPPacket constructor when object deserialization from data fails,
 * i.e. when e.g. data is too short for creating header, or data values are invalid.
 *
 * @see RTPPacket
 * @see RTPHeader
 */
class DeserializationException: public exception {
public:
	DeserializationException();
	virtual ~DeserializationException() throw();
};

#endif /* DESERIALIZATIONEXCEPTION_H_ */
