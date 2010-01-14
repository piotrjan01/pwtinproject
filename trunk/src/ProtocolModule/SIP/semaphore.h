/*
 * semaphore.h
 * Klasa semafor, dla ulatwienia synchronizacji.
 */

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

/*#include <sys/types.h> 
#include <sys/stat.h> 
#include <string.h> 
#include <errno.h> 
#include <fcntl.h> 
#include <pthread.h> 
#include <unistd.h>*/

#include <semaphore.h>

class Semaphore {

	private:
		sem_t sem;

	public:
		Semaphore( int value ) {
			if( sem_init( & sem, 0, value ) != 0 )
				throw "sem_init: failed";
		}

		~Semaphore() { 
			sem_destroy( & sem ); 
		}

		void P() {
			if( sem_wait( & sem ) != 0 )
				throw "sem_wait: failed";
		}

		void V() {
			if( sem_post( & sem ) != 0 )
				throw "sem_post: failed";
		}
};

#endif
