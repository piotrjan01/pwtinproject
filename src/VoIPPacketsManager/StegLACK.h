/**
 * @file StegLACK.h
 *
 * \~
 * \~polish
 * \~english
 * \~
 * @date created 2010-01-14
 * @author Piotr Gwizdała
 */
#ifndef STEGLACK_H_
#define STEGLACK_H_

#include <vector>
#include "VoIPPacketsManager.h"
#include "../Util/Timer.h"
#include <queue>

/**
 * \~
 * \~polish
 * Element sekwencji danych steganograficznych.
 * \~english
 * Steganographic sequence element.
 * \~
 */
class StegSeqElem {
public:
	StegSeqElem(string stegSeq);
	int intervMin;
	int intervMax;
	bool isStegPacket;
};

/**
 * \~
 * \~polish
 * Imlementacja @ref VoIPPacketsManager wysyłająca dane steganograficzne.
 *
 * Dzieli dane audio jak @ref NoSteg, ale dodatkowo wplata w pakiety audio
 * pakiety z danymi  steganograficznymi. W przypadku odbioru informacji, posiada
 * odpowiednio powiększony bufor i nie opuszcza pakietów, co umożliwia odczytanie danych
 * steganograficznych.
 *
 * \~english
 * Implementation of @ref VoIPPacketsManager that sends steganographic data.
 *
 * Differs from @ref NoSteg by sending not only audio data but also
 * steganographic data.
 * \~
 * @see NoSteg
 */
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
	 * Used when simulating the incoming packets queue. Says how much space is left in incoming packets queue.
	 */
	int queueFreeSpace;

	/**
	 * Counts the time that passed since last packet was read from the incoming queue
	 */
	Timer timeSinceLastQueueRead;

	/**
	 * Incoming packets queue
	 */
	vector<RTPPacket> incQueue;

	/**
	 * The packets that didn't fit to queue, so they may be steganographic packets.
	 */
	queue<RTPPacket> stegPackets;

	/**
	 * The sequence we will use to send our steganographic data
	 */
	vector<StegSeqElem> stegSeq;

	long getRandNumber(long min, long max);

	StegLACK(Config* cfg);
	virtual ~StegLACK();

	virtual RTPPacket& getNextPacket();
	virtual void putReceivedPacketData(RTPPacket& packet);

	void readStegDataToMem();

	vector<char> getStegDataToSend();
private:
	RTPPacket templatePacket;
};

#endif /* STEGLACK_H_ */
