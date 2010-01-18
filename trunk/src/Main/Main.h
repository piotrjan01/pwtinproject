/**
 * @file Main.h
 * @brief Observer design pattern.
 *
 * Interface of the observer.
 *
 * @date created 2010-01-14
 * @author Piotr Gwizdała
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <cstdlib>
#include <csignal>
#include <string>

#include "Config.h"
#include "../Exceptions/BindException.h"

using namespace std;

void mainSignalHandler(int signal);

/**
 * \~polish
 * Klasa startowa -- singleton.
 *
 * Klasa startowa zarządzająca startem programu. Wywołuje moduł
 * wczytywania ustawień @ref Config. Przechwytuje sygnały.
 * Wywołuje @ref VoIPModule.
 * Obsługuje błędy, loguje do plików etc.
 *
 * Wykorzystuje wzorzec projektowy Singleton.
 *
 * @see http://pl.wikipedia.org/wiki/Singleton_(wzorzec_projektowy)
 *
 * \~english
 * Main starting class -- singleton.
 *
 * Starting class responsible for delegation of reading configuration files ,
 * audio and steganography data to @ref Config.
 * Invokes @ref VoIPModule that handles the transmission.
 *
 * Uses Singleton design pattern.
 *
 * @see http://en.wikipedia.org/wiki/Singleton_pattern
 *
 * \~
 * @see Config
 * @see VoIPModule
 */
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
	void info(string message);

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
