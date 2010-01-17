/*
 * StegLACK.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef STEGLACK_H_
#define STEGLACK_H_

#include <vector>
#include "VoIPPacketsManager.h"

/**
 * Steganographic sequence element
 */
class StegSeqElem {
public:
	StegSeqElem(string stegSeq);
	int intervMin;
	int intervMax;
	bool isStegPacket;
};

class StegLACK: public VoIPPacketsManager {
public:

	/**
	 * Counts normal packages being sent. Once intervalCount reaches seqFireInterval, the sequence is
	 * being send according to its structure stored in stegSeq.
	 */
	int intervalCount;


	/**
	 * The interval at which the sequence fires. After it fires, the new seq interval is being
	 * randomly chosen according to min and max intervals from config.
	 */
	int seqFireInterval;

	/**
	 * Stores the current position in the stegSeq sequence
	 */
	int seqPosition;

	/**
	 * Steganographic data read from a file
	 */
	vector<char> stegData;

	/**
	 * Pointer to remember the position in stegData
	 */
	int lastReadStegByte;

	/**
	 * Flags wether steg data has been already transfered
	 */
	bool stegTransferDone;


	/**
	 * The sequence we will use to send our steganographic data
	 */
	vector<StegSeqElem> stegSeq;

	long getRandNumber(long min, long max);

	StegLACK(Config* cfg);
	virtual ~StegLACK();

	virtual RTPPacket& getNextPacket();
	virtual void putReceivedPacketData(char* data, int dataSize);

	void readStegDataToMem();

	vector<char> getStegDataToSend();
private:
	RTPPacket templatePacket;
};

#endif /* STEGLACK_H_ */
