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


#include "VoIPPacketsManager.h"
#include "../Util/Timer.h"
#include <queue>
#include <vector>

/**
 * It is used to flag the steganographic packets. The payload will start with this string.
 */
#define STEG_DATA_FLAG "steg"

/**
 * The length of string defined by STEG_DATA_FLAG
 */
#define STEG_DATA_FLAG_SIZE 4

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
 * Implementacja @ref VoIPPacketsManager wysyłająca dane steganograficzne.
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



	StegLACK(Config* cfg);
	virtual ~StegLACK();

	virtual RTPPacket& getNextPacket();
	virtual void putReceivedPacketData(RTPPacket& packet);



private:
	/**
	 * Sprawdza czy pakiet jest oflagowany jako steganograficzny i jeśli tak
	 * to go zapisuje do odpowiedniego pliku podanego w konfiguracji.
	 */
	void saveIfStegPacket(RTPPacket &p);

	void readStegDataToMem();

	long getRandNumber(long min, long max);

	vector<char> getStegDataToSend();

private:
	RTPPacket templatePacket;
};

#endif /* STEGLACK_H_ */
