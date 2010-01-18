/**
 * @file Observer.h
 * @brief Observer design pattern.
 *
 * Interface of the observer.
 *
 * @date created 2010-01-17
 * @author Marek
 * @see Observable.h
 * @see http://en.wikipedia.org/wiki/Observer_pattern
 */

#ifndef OBSERVER_H_
#define OBSERVER_H_

using namespace std;

/**
 * Interface of the observer
 *
 * @see Observable
 * @see http://en.wikipedia.org/wiki/Observer_pattern
 */
class Observer {
public:
	/**
	 * Virtual destructor with empty body
	 */
	virtual ~Observer() {
	}

	/**
	 * Method invoked on Observable state change.
	 *
	 * Method invoked by the Observable on the Observer, when the state of Observable has been changed.
	 */
	virtual void update() = 0;
};

#endif /* OBSERVER_H_ */
