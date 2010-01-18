/**
 * @file Timer.h
 *
 * @date created 2010-01-17
 * @author Piotr Gwizdała
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

/**
 * \~
 *
 * \~polish
 * Pomocnicza klasa używana do odmierzania czasu.
 *
 * \~english
 * A wrapper commodity class for time counting.
 *
 * \~
 */
class Timer {

	struct timeval begin;
	struct timeval end;

public:
	static const long MILISECONDS_IN_A_SECOND = 1000;
	static const long USECONDS_IN_A_MILISECOND = 1000;

	/**
	 * \~
	 * \~polish
	 * \~english
	 * \~
	 */
	/**
	 * \~
	 * \~polish
	 * Resetuje i startuje stoper.
	 * \~english
	 * Starts (or actually restarts) the timer.
	 * \~
	 */
	void start() {
		gettimeofday(&begin, NULL);
	}

	/**
	 * \~
	 *
	 * \~polish
	 * Zwraca czas w milisekundach, który upłynął od startu stopera.
	 * @return różnica podana w milisekundach pomiędzy czasem obecnym a czasem ostatniego wywołania metody #start().
	 *
	 * \~english
	 * Returns time that has elapsed since the last invocation of #start() method.
	 * @return time span in milliseconds that has elapsed since the last invocation of #start() method.
	 *
	 * \~
	 */
	long seeTime() {
		struct timeval _end;
		gettimeofday(&_end, NULL);

		long seconds = _end.tv_sec - begin.tv_sec;
		long useconds = _end.tv_usec - begin.tv_usec;
		long mtime = (seconds * MILISECONDS_IN_A_SECOND + useconds
				/ USECONDS_IN_A_MILISECOND);

		return (double) mtime;
	}

	/**
	 * \~
	 *
	 * \~polish
	 * Zwraca czas w milisekundach, który upłynął od startu stopera i zatrzymuje stoper.
	 * @return różnica podana w milisekundach pomiędzy czasem obecnym a czasem ostatniego wywołania metody #start().
	 *
	 * \~english
	 * Returns time that has elapsed since the last invocation of #start() method and stops the timer.
	 * @return time span in milliseconds that has elapsed since the last invocation of #start() method.
	 *
	 * \~
	 */
	long stop() {
		gettimeofday(&end, NULL);

		long seconds = end.tv_sec - begin.tv_sec;
		long useconds = end.tv_usec - begin.tv_usec;
		long mtime = (seconds * MILISECONDS_IN_A_SECOND + useconds
				/ USECONDS_IN_A_MILISECOND);

		return (double) mtime;
	}
};

#endif /* TIMER_H_ */
