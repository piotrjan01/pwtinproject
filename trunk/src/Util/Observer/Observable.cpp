/*
 * Observable.cpp
 *
 *  Created on: 2010-01-17
 *      Author: marek
 */

#include "Observable.h"

Observable::~Observable() {
}

void Observable::addObserver(Observer & o) {
	observers.push_back(&o);
}

void Observable::removeObserver(Observer & o) {
	observers.remove(&o);
}

void Observable::notifyObservers() {
	list<Observer*>::iterator it;
	for (it = observers.begin(); it != observers.end(); ++it) {
		(*it)->update();
	}
}

