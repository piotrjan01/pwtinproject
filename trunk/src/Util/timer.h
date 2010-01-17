/*
 * timer.h
 *
 *  Created on: Jan 17, 2010
 *      Author: piotrrr
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <time.h>

class Timer {
	clock_t t;
public:
	static const long USECONDS_IN_A_MILISECOND = 1000;
	/**
	 * Resetuje i startuje stoper.
	 */
	void start() {
		t=clock();
	}

	/**
	 * Zwraca milisekundy od startu stopera
	 */
	double seeTime() {
		double ret=clock()-t;
		return (double)(1000.0*ret / (double)CLOCKS_PER_SEC);
	}


	/**
	 * Zwraca milisekundy i zatrzymuje stoper
	 */
	double stop() {
		t=clock()-t;
		return (double)(1000.0*t / (double)CLOCKS_PER_SEC);
	}
};

#endif /* TIMER_H_ */
