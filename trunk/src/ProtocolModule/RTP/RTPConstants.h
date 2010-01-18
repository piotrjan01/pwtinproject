/**
 * @file RTPConstants.h
 *
 *
 * @date created 2010-01-16
 * @author: Marek
 * @see RTPHeader.h
 */

#ifndef RTPCONSTANTS_H_
#define RTPCONSTANTS_H_

typedef signed char qint8;
typedef unsigned char quint8;
typedef short qint16;
typedef unsigned short quint16;
typedef int qint32;
typedef unsigned int quint32;
typedef long long qint64;
typedef unsigned long long quint64;
typedef qint64 qlonglong;
typedef quint64 qulonglong;

static const quint16 RTP_PORT_NUMBER = 5004;

// timestamp interval for sequence of RTP packets is dependent on the sample rate
/* for Audio 8 kHz - sample rate */
static const quint32 TIMESTAMP_INTERVAL_8KHZ = 160;
/* for Audio 16 kHz - sample rate */
static const quint32 TIMESTAMP_INTERVAL_16KHZ = 320;

// tolerant range of lost packet to indicate to increment cycle number
static const quint8 RANGE_TO_INDICATE_NEW_CYCLE_NUMBER = 10;

enum SampleRate {
	NARROW_BAND = 8000, WIDE_BAND = 16000
};

enum PayloadType {
	//	PT	Name	Type	Clock rate (Hz)	Audio channels	References

	//	0	PCMU	Audio	8000	1	RFC 3551
	PCMU = 0,
	G711_PCMU = 0,
//	1	1016	Audio	8000	1	RFC 3551
//	2	G721	Audio	8000	1	RFC 3551
//	3	GSM	Audio	8000	1	RFC 3551
//	4	G723	Audio	8000	1
//	5	DVI4	Audio	8000	1	RFC 3551
//	6	DVI4	Audio	16000	1	RFC 3551
//	7	LPC	Audio	8000	1	RFC 3551
//	8	PCMA	Audio	8000	1	RFC 3551
//	9	G722	Audio	8000	1	RFC 3551
//	10	L16	Audio	44100	2	RFC 3551
//	11	L16	Audio	44100	1	RFC 3551
//	12	QCELP	Audio	8000	1
//	13	CN	Audio	8000	1	RFC 3389
//	14	MPA	Audio	90000		RFC 2250, RFC 3551
//	15	G728	Audio	8000	1	RFC 3551
//	16	DVI4	Audio	11025	1
//	17	DVI4	Audio	22050	1
//	18	G729	Audio	8000	1
//	19	reserved	Audio
//	20
//	-
//	24
//	25	CellB	Video	90000	 	RFC 2029
//	26	JPEG	Video	90000	 	RFC 2435
//	27
//	28	nv	Video	90000	 	RFC 3551
//	29
//	30
//	31	H261	Video	90000	 	RFC 2032
//	32	MPV	Video	90000	 	RFC 2250
//	33	MP2T	Audio/Video	90000	 	RFC 2250
//	34	H263	Video	90000
//	35
//	-
//	71
//	72
//	-
//	76	reserved	 	 	 	RFC 3550
//	77
//	-
//	95
//	96
//	-
//	127	dynamic	 	 	 	RFC 3551
};

#endif /* RTPCONSTANTS_H_ */
