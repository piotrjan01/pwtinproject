/**
 * @file Observable.h
 * @brief Observer design pattern.
 *
 * Interface of the subject (observed / observable).
 *
 * @date created 2010-01-17
 * @author Marek
 * @see Observer.h
 * @see http://en.wikipedia.org/wiki/Observer_pattern
 */

#ifndef OBSERVABLE_H_
#define OBSERVABLE_H_

#include <iterator>
#include <list>

#include "Observer.h"

/**
 * Interface of the observed object (observable / subject)
 *
 * @see Observer
 * @see http://en.wikipedia.org/wiki/Observer_pattern
 */
class Observable {
public:
	virtual ~Observable();

	/**
	 * Adds observer to the list of notified observers.
	 *
	 * @param o Observer object to be added
	 */
	virtual void addObserver(Observer& o);

	/**
	 * Removes observer from the list of notified observers.
	 *
	 * @param o Observer object to be removed
	 */
	virtual void removeObserver(Observer& o);
protected:
	/**
	 * Notifies all objects included in the list of notified observers.
	 *
	 * Notifies all objects included in the list of notified observers by
	 * invoking their @ref Observer#update "update" method.
	 *
	 * @see Observer#update
	 */
	virtual void notifyObservers();

	/**
	 * List of all observers.
	 */
	std::list<Observer*> observers;
};

#endif /* OBSERVABLE_H_ */
