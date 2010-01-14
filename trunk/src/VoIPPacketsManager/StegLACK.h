/*
 * StegLACK.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#ifndef STEGLACK_H_
#define STEGLACK_H_

#include "VoIPPacketsManager.h"

class StegLACK: public VoIPPacketsManager {
public:
	StegLACK();
	virtual ~StegLACK();
};

#endif /* STEGLACK_H_ */
