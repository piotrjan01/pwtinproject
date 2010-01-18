/**
 * @file Observable.cpp
 * @brief Observer design pattern.
 *
 * Interface of the subject (observed / observable).
 *
 * @date created 2010-01-17
 * @author Marek
 * @see Observer.h
 * @see http://en.wikipedia.org/wiki/Observer_pattern
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

