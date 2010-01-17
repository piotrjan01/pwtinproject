/*
 * Main.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <cstdlib>
#include <csignal>
#include <string>

#include "Config.h"

using namespace std;

void mainSignalHandler(int signal);

class Main {
	friend void mainSignalHandler(int signal);
public:
	virtual ~Main();

	static Main* getInstance();

	bool isHangUp();

	/**
	 * Tak na prawd� jedyne co mo�emy zrbi� ze string to wy�wietli�, zalogowa� lub nic.
	 */
	void handleError(string message);

	/**
	 * Tu przekazujemy argumenty wywo�ania programu.
	 */
	int startProgram(int argc, char **argv);

	void printUsage();

private:
	Config* cnf;
	void setSignalHandlers();
	bool hangUp;

	typedef void (*sighandler_t)(int);

	Main();
	static Main* instance;

};

#endif /* MAIN_H_ */
