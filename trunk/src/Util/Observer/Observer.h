/*
 * Observer.h
 *
 *  Created on: 2010-01-17
 *      Author: marek
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

using namespace std;

class Observer {
public:
	virtual ~Observer() {}
	virtual void update() = 0;
};

#endif /* OBSERVER_H_ */
