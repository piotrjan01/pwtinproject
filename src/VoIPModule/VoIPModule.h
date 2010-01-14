/*
 * VoIPModule.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#ifndef VOIPMODULE_H_
#define VOIPMODULE_H_

#include <string>
#include "debug/debug.h"

using namespace std;

class VoIPModule {
public:
	VoIPModule();
	virtual ~VoIPModule();

	void doSending(string audioFile, int destIP);

	void doReceiving(string audioFile);

};

#endif /* VOIPMODULE_H_ */
