/*
 * Main.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string>

using namespace std;

class Main {
public:
	Main();
	virtual ~Main();

	/**
	 * Tak na prawdê jedyne co mo¿emy zrbiæ ze string to wyœwietliæ, zalogowaæ lub nic.
	 */
	static void handleError(string message);

	/**
	 * Tu przekazujemy argumenty wywo³ania programu.
	 */
	static int startProgram(int argc, char **argv);

};

#endif /* MAIN_H_ */
