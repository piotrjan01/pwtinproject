/*
 * main.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include <cstdlib>

#include "RTPAgent.h"
#include "RTPPacket.h"

int _main(int argc, char *argv[]) {
	PRN("--------------- MAIN --------------");
	RTPAgent rtpAgent;
	RTPPacket rtpPacket;
	cout << *rtpPacket.header;

	return EXIT_SUCCESS;
}
