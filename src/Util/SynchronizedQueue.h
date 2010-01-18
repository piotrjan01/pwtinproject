/*
 * \~
 * @file SynchronizedQueue.h
 *
 * \~english
 * A thread-safe (synchronized) queue for <code>RTPPacket</code>s header file.
 *
 * \~
 * @date created 2010-01-16
 * @author: Marek
 * @see RTPPacket.h
 */

#ifndef SYNCHRONIZEDQUEUE_H_
#define SYNCHRONIZEDQUEUE_H_

#include <queue>
#include <deque>

#include "../ProtocolModule/RTP/RTPPacket.h"

#include "../Util/Semaphore.h"

typedef unsigned int size_type;

/**
 * \~english
 * A thread-safe (synchronized) queue for <code>RTPPacket</code>s
 *
 * \~polish
 * Synchronizowana kolejka nieblokująca dla pakietów RTP
 */
class SynchronizedQueue {
public:
	typedef RTPPacket* value_type;
	typedef std::deque<value_type> Container;

	//	Construct queue
	explicit SynchronizedQueue(const Container& ctnr = Container());
	virtual ~SynchronizedQueue();

	//	Test whether container is empty
	bool empty();

	//	Return size
	size_type size();

	//	Insert element at the end of the queue
	void pushBack(const value_type& x);

	//	Retrieve next element and remove it from queue
	value_type& popFront();

private:
	std::queue<value_type> queue;
	Semaphore mutex;
	//	Semaphore wait;
};

#endif /* SYNCHRONIZEDQUEUE_H_ */
