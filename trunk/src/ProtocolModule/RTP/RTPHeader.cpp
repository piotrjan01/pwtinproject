/*
 * RTPHeader.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include "RTPHeader.h"

ostream& RTPHeader::toStream(ostream& os) {
	char value[SIZE_IN_BYTES];

	value[0] = ((v << 6) & 0xc0) | ((((quint8) p) << 5) & 0x20)
			| ((((quint8) x) << 4) & 0x10) | (cc & 0x0f);
	value[1] = (((((quint8) m) << 7) & 0x80) | (pt & 0x7f));

	value[2] = (sequenceNumber >> 8) & 0xFF;
	value[3] = (sequenceNumber) & 0xFF;

	value[4] = (timestamp >> 24) & 0xFF;
	value[5] = (timestamp >> 16) & 0xFF;
	value[6] = (timestamp >> 8) & 0xFF;
	value[7] = (timestamp) & 0xFF;

	value[8] = (ssrc >> 24) & 0xFF;
	value[9] = (ssrc >> 16) & 0xFF;
	value[10] = (ssrc >> 8) & 0xFF;
	value[11] = (ssrc) & 0xFF;
	os.write(value, SIZE_IN_BYTES);
	return os;
}

string RTPHeader::toString() {
	ostringstream oss;
	toStream(oss);
	return oss.str();
}

RTPHeader::~RTPHeader() {
	// TODO Auto-generated destructor stub
}

RTPHeader::RTPHeader(char* data) {
	v = ((data[0] >> 6) & 0x03);
	p = ((data[0] >> 5) & 0x01);
	x = ((data[0] >> 4) & 0x01);
	cc = ((data[0]) & 0x0F);
	m = ((data[1] >> 7) & 0x01);
	pt = ((data[1] >> 1) & 0x7f);
	sequenceNumber = data[2];
	timestamp = data[4];
	ssrc = data[8];
}

RTPHeader::RTPHeader() :
	v(USED_RTP_VERSION), p(false), x(false), cc(0), m(false), pt(G711_PCMU),
			sequenceNumber(1), timestamp(0), ssrc(0) {
	sequenceNumberCycle = 0;
}

RTPHeader::RTPHeader(const RTPHeader& another) :
	v(another.v), p(another.p), x(another.x), cc(another.cc), m(another.m), pt(
			another.pt), sequenceNumber(another.sequenceNumber), timestamp(
			another.timestamp), ssrc(another.ssrc) {
	sequenceNumberCycle = another.sequenceNumberCycle;
}

RTPHeader::RTPHeader(quint8 _v, bool _p, bool _x, quint8 _cc, bool _m,
		quint8 _pt, quint16 _sequenceNumber, quint32 _timestamp, quint32 _ssrc) :
	v(_v), p(_p), x(_x), cc(_cc), m(_m), pt(_pt), sequenceNumber(
			_sequenceNumber), timestamp(_timestamp), ssrc(_ssrc) {
	sequenceNumberCycle = 0;
}

RTPHeader::RTPHeader(SampleRate _sampleRate, quint16 _sequenceNumber,
		quint32 _timestamp, quint32 _ssrc) :
	v(2), p(true), x(false), cc(0), m(false), pt(G711_PCMU), sequenceNumber(
			_sequenceNumber), timestamp(_timestamp), ssrc(_ssrc) {
	sequenceNumberCycle = 0;
}

RTPHeader & RTPHeader::operator=(const RTPHeader& another) {
	v = another.v;
	p = another.p;
	x = another.x;
	cc = another.cc;
	m = another.m;
	pt = another.pt;
	sequenceNumber = another.sequenceNumber;
	timestamp = another.timestamp;
	ssrc = another.ssrc;
	sequenceNumberCycle = another.sequenceNumberCycle;
	return *this;
}

//***********************************************
void RTPHeader::nextRTPHeader(SampleRate _sampleRate) {
	int difference = 0;
	// change pt (payload type) and timestamp
	if (_sampleRate == NARROW_BAND) {
		if (timestamp + TIMESTAMP_INTERVAL_8KHZ <= UINT_MAX) {
			timestamp += TIMESTAMP_INTERVAL_8KHZ;
		} else {
			difference = UINT_MAX - timestamp;
			timestamp = TIMESTAMP_INTERVAL_8KHZ - difference - 1; // count from zero
		}
	} else { // if(_sampleRate == WIDE_BAND)
		if (timestamp + TIMESTAMP_INTERVAL_16KHZ <= UINT_MAX) {
			timestamp += TIMESTAMP_INTERVAL_16KHZ;
		} else {
			difference = UINT_MAX - timestamp;
			timestamp = TIMESTAMP_INTERVAL_16KHZ - difference - 1; // count from zero
		}
	}
	// change sequence number
	if (sequenceNumber + 1 > USHRT_MAX) {
		sequenceNumber = 0;
		++sequenceNumberCycle;
	} else {
		++sequenceNumber;
	}

}

std::ostream& operator<<(std::ostream& str, RTPHeader head) {
	str << "Version: " << (unsigned) head.v << std::endl;
	str << "Padding: " << (bool) head.p << std::endl;
	str << "Extension: " << head.x << std::endl;
	str << "CSRC Count: " << (unsigned) head.cc << std::endl;
	str << "Marker: " << head.m << std::endl;
	str << "Payload Type: " << (unsigned) head.pt << std::endl;
	str << "Sequence Number: " << head.sequenceNumber << std::endl;
	str << "Timestamp: " << head.timestamp << std::endl;
	str << "SSRC: " << head.ssrc << std::endl;

	return str;
}

//***********************************************
quint16 RTPHeader::generateSequenceNumber() {
	srand(time(NULL));
	return (quint16) (rand() % USHRT_MAX + 1); // unsigned short max value
}

//***********************************************
quint32 RTPHeader::generateTimestamp() {
	srand(time(NULL));
	return (quint32) (rand() % ULONG_MAX + 1); // unsigned long max value
}

//***********************************************
quint32 RTPHeader::generateSSRC() {
	srand(time(NULL));
	return (quint32) (rand() % ULONG_MAX + 1); // unsigned long max value
}

