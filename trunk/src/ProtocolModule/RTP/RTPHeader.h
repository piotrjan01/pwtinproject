/**
 * @file RTPHeader.h
 *
 *
 * @date created 2010-01-16
 * @author: Marek
 */

#ifndef RTPHEADER_H_
#define RTPHEADER_H_

#include <climits>
#include <cstdlib>

#include <string>
#include <sstream>

#include "RTPConstants.h"

using namespace std;

/**
 * RTP packet header content.
 *
 * Class for storing, manipulating and creating the content of RTP packet header.
 *
 * @see http://www.networksorcery.com/enp/protocol/rtp.htm
 *
 * @code
 * 01 2 3 4567 0123 4567
 * == = = ==== ==== ====
 * v  p x [cc] m[  pt  ]
 * [   sequenceNumber  ]
 * [      timestamp
 *        timestamp    ]
 * [        ssrc
 *          ssrc       ]
 *
 * optionally: from 0 to 15 times
 * [        csrc
 *          csrc       ]
 *  @endcode
 *  @see RTPPacket.h
 */

/**
 * The object of the class RTPHeader stores, manipulates and generates information about RTP header.
 *
 *  @see RTPPacket
 */
class RTPHeader {
	friend std::ostream& operator<<(std::ostream& str, RTPHeader head);
	static const int USED_RTP_VERSION = 2;
public:
	RTPHeader();
	RTPHeader(char* data);
	virtual ~RTPHeader();

	static const quint32 SIZE_IN_BYTES = 12;

	ostream& toStream(ostream& os);
	string toString();

	/* version 2 bits  - it is always set to the value 2, which is the current version of RTP */
	quint8 v;
	/* padding 1 bit - is set when the packet contains one or more additional padding octets at the end which are not part of the payload - for this project it should be set to zero */
	bool p;
	/* extension 1 bit - is set when the header is followed by exactly one header extension - for this project it should be zero */
	bool x;
	/* CSRC count 4 bits - contains the number of CSRC identifiers that follow the fixed header - for this project it should be zero*/
	quint8 cc;

	/* marker 1 bit - the interpretation of the marker is dependent on the payload type */
	bool m;

	/* payload type 7 bits - this field identifies the format of the RTP payload and determines its interpretation by the application - it should be value 5 or 6 */
	quint8 pt;

	/* sequence number 16 bits - the sequence number increments by one for each RTP data packet sent */
	quint16 sequenceNumber;

	/* timestamp 32 bits - the timestamp reflects the sampling instant of the first octet in the RTP data packet */
	quint32 timestamp;

	/* the SSRC synchronization source 32 bits - identifies the synchronization source */
	quint32 ssrc;

	/* counts how many times there was passing through zero for sequence number */
	quint8 sequenceNumberCycle;

public:
	/* copying constructor */
	RTPHeader(const RTPHeader& another);

	/* RTPHeader all constructor */
	RTPHeader(quint8 _v, bool _p, bool _x, quint8 _cc, bool _m, quint8 _pt,
			quint16 _sequenceNumber, quint32 _timestamp, quint32 _ssrc);

	/* RTP standard constructor */
	RTPHeader(SampleRate _sampleRate, quint16 _sequenceNumber,
			quint32 _timestamp, quint32 _ssrc);

	/* RTP assignment operator */
	RTPHeader & operator=(const RTPHeader& another);

	/* change actual RTP header to new next packet */
	void nextRTPHeader(SampleRate _sampleRate);

	/* to generate appropriate sequenceNumber */
	static quint16 generateSequenceNumber();

	/* to generate appropriate timestamp */
	static quint32 generateTimestamp();

	/* to generate appropriate synchronization source identifier -- actually should be unique and conflicts should be resolved */
	static quint32 generateSSRC();

#if 0
	/* indicates that there is padding or not */
	bool isPadding();

	/* returns payload type */
	quint8 getPayloadType();

	/* returns sequence number */
	quint16 getSequenceNumber();

	/* returns timestamp */
	quint32 getTimestamp();

	/* return SSRC */
	quint32 getSSRC();

	/* returns cycle of sequence number */
	quint8 getSequenceNumberCycle();

	/* set cycle of sequence number */
	void setSequenceNumberCycle(quint8 _sequenceNumberCycle);

	/* to write header data to QdataStream */
	friend QDataStream& operator<<(QDataStream& str, RTPHeader & head);

	/* to write header data to standard stream */
	friend std::ostream& operator<<(std::ostream& str, RTPHeader head);

	/* to read header data from QDataStream */
	friend QDataStream& operator>>(QDataStream& str, RTPHeader & head);

#endif
};

#endif /* RTPHEADER_H_ */
