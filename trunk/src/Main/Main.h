/*
 * Main.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include "Config.h"

using namespace std;

class Main {
public:
	Main();
	virtual ~Main();

	Config* cnf;

	/**
	 * Tak na prawd� jedyne co mo�emy zrbi� ze string to wy�wietli�, zalogowa� lub nic.
	 */
	static void handleError(string message);

	/**
	 * Tu przekazujemy argumenty wywo�ania programu.
	 */
	static int startProgram(int argc, char **argv);

};

#endif /* MAIN_H_ */
