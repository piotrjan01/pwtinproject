/*
 * NoSteg.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#ifndef NOSTEG_H_
#define NOSTEG_H_

#include "VoIPPacketsManager.h"

class NoSteg: public VoIPPacketsManager {
public:
	NoSteg();
	virtual ~NoSteg();
};

#endif /* NOSTEG_H_ */
