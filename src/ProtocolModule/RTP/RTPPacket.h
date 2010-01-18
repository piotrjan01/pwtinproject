/**
 * @file RTPPacket.h
 *
 *
 * @date created 2010-01-16
 * @author: Marek
 * @see RTPHeader.h
 */

#ifndef RTPPACKET_H_
#define RTPPACKET_H_

#include "RTPHeader.h"
#include "DeserializationException.h"

#include <string>
#include <cstring>

using namespace std;

/**
 * RTP packet content
 *
 * RTP packet includes RTP header and payload data.
 * RTPPacket object stores and manipulates RTP packet data as a whole -- delegating manipulation
 * on header to @ref RTPHeader.
 *
 * @see RTPHeader
 */
class RTPPacket {
	static const int DEFAULT_DELAY = 0;
public:
	/**
	 * Default constructor.
	 *
	 * Creates an RTPPacket object with no data and default values set in the header.
	 *
	 * @see also RTPHeader#RTPHeader()
	 */
	RTPPacket();
	/**
	 * Copying constructor.
	 *
	 * Creates an object being a copy having header and payload data from <code>another</code> RTPPacket.
	 */
	RTPPacket(const RTPPacket& another);

	RTPPacket(char* packetData, int packetDataSize);
	RTPPacket(const RTPHeader& hdr, char* payloadData, int payloadDataSize);
	virtual ~RTPPacket();

	/**
	 * Writes packet as binary data to the stream.
	 *
	 * @param os <code>ostream</code> to be used for writing.
	 */
	ostream& toStream(ostream& os);

	/**
	 * Writes packet as binary data to a string.
	 *
	 * A commodity function. Effectively calls @ref #toStream(ostream&) "toStream(std::ostringstream&).str()".
	 */
	string toString();

	RTPPacket& operator=(const RTPPacket& another);

	long delay; ///< Delay in milliseconds that should be used for sending the packet

	RTPHeader* header; ///< RTP Packet header

	char* payload; ///< Payload -- audio or video encoded data
	unsigned payloadSize; ///< Size of payload.
};

#endif /* RTPPACKET_H_ */
