/*
 * SynchronizedQueue.cpp
 *
 *  Created on: 2010-01-16
 *      Author: marek
 */

#include "SynchronizedQueue.h"

SynchronizedQueue::SynchronizedQueue(const Container& ctnr) :
	queue(ctnr), mutex(0)
//, wait(0)
{

	mutex.V(); // to be done in the very end
}

SynchronizedQueue::~SynchronizedQueue() {
	// TODO Auto-generated destructor stub
}

size_type SynchronizedQueue::size() {
	mutex.P();
	size_type result = queue.size();
	mutex.V();
	return result;
}

RTPPacket*& SynchronizedQueue::popFront() {
	mutex.P();
	//	wait.P();
	value_type& result = queue.front();
	queue.pop();
	mutex.V();
	return result;
}

void SynchronizedQueue::pushBack(const value_type& x) {
	mutex.P();
	//	wait.V();
	queue.push(const_cast<value_type> (x));
	mutex.V();
}

bool SynchronizedQueue::empty() {
	mutex.P();
	bool result = queue.empty();
	mutex.V();
	return result;
}

