/*
 * Main.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "Main.h"
#include "../debug/debug.h"

Main* Main::main = 0;

Main::Main() {
	if (Main::main != NULL) return;
	main = this;
}

Main::~Main() {
	main = NULL;
}

Main* Main::getMain() {
	if (main != NULL) return main;
	main = new Main();
	return main;
}


void Main::handleError(string message) {
	PRN("Error: ");
	PRN(message);
}

int Main::startProgram(int argc, char **argv) {
	PRN1("in startProgram");

	string configFile;
	//pierwszy argument to nazwa/sciezka do pliku z config.

	if (argc != 2) Main::handleError("Too little arguments!");
	configFile = argv[1];

	cnf = new Config(configFile);

	PRN("TODO: wczytuje dane audio");



	PRN("TODO: wywo�uje odpowiednie metody w VoIPModule..");

	PRN("TODO: zapisuje loga do pliku...");

	PRN1("ends startProgram");
	return 0;
}
