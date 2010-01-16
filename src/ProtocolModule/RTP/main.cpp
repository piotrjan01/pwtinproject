/*
 * main.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include <cstdlib>

//#include "RTPAgent.h"
#include "RTPPacket.h"
#include "RTPHeader.h"

int main(int argc, char *argv[]) {
	PRN("--------------- MAIN --------------");
	//	RTPAgent rtpAgent;

	RTPHeader hdr;
	RTPPacket rtpPacket;
	rtpPacket.header = &hdr;

	ostringstream oss;
	rtpPacket.toStream(oss);
	PRNBITS(oss.str());
	return EXIT_SUCCESS;
}
