/*
 * Observable.h
 *
 *  Created on: 2010-01-17
 *      Author: marek
 */

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <iterator>
#include <list>

#include "Observer.h"

class Observable {
public:
	Observable();
	virtual ~Observable();
	virtual void addObserver(Observer& o);
	virtual void removeObserver(Observer& o);
protected:
	virtual void notifyObservers();
	std::list<Observer*> observers;
};

#endif /* OBSERVABLE_H_ */
