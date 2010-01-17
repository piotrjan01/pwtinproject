/*
 * DeserializationException.h
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#ifndef DESERIALIZATIONEXCEPTION_H_
#define DESERIALIZATIONEXCEPTION_H_

#include <exception>

using namespace std;

class DeserializationException: public exception {
public:
	DeserializationException();
	virtual ~DeserializationException() throw();
};

#endif /* DESERIALIZATIONEXCEPTION_H_ */
