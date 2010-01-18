/*
 * timer.h
 *
 *  Created on: Jan 17, 2010
 *      Author: piotrrr
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

class Timer {

	struct timeval begin;
	struct timeval end;


public:
	static const long MILISECONDS_IN_A_SECOND = 1000;
	static const long USECONDS_IN_A_MILISECOND = 1000;



	/**
	 * Resetuje i startuje stoper.
	 */
	void start() {
		gettimeofday(&begin, NULL);
	}

	/**
	 * Zwraca milisekundy od startu stopera
	 */
	long seeTime() {
		struct timeval _end;
		gettimeofday(&_end, NULL);

		long seconds = _end.tv_sec - begin.tv_sec;
		long useconds = _end.tv_usec - begin.tv_usec;
		long mtime = (seconds*MILISECONDS_IN_A_SECOND + useconds / USECONDS_IN_A_MILISECOND);

		return (double) mtime;
	}

	/**
	 * Zwraca milisekundy i zatrzymuje stoper
	 */
	long stop() {
		gettimeofday(&end, NULL);

		long seconds = end.tv_sec - begin.tv_sec;
		long useconds = end.tv_usec - begin.tv_usec;
		long mtime = (seconds*MILISECONDS_IN_A_SECOND + useconds / USECONDS_IN_A_MILISECOND);

		return (double) mtime;
	}
};

#endif /* TIMER_H_ */
