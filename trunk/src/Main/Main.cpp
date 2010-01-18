/*
 * Main.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "Main.h"
#include "../debug/debug.h"
#include "../VoIPModule/VoIPModule.h"

Main* Main::instance = 0;


void mainSignalHandler(int signal) {
	VAR_(2, signal);
	if (signal == SIGINT) {
		PRN_(2, "CTRL-C pressed (signal SIGINT)")
		Main::getInstance()->hangUp = true;
	}
}

Main::Main() {
	instance = this;
	hangUp = false;
	setSignalHandlers();
}

bool Main::isHangUp() {
	return hangUp;
}

void Main::setSignalHandlers() {
	signal(SIGINT, mainSignalHandler);
}

Main::~Main() {
	instance = NULL;
}

Main* Main::getInstance() {
	if (instance != NULL)
		return instance;
	instance = new Main();
	return instance;
}

void Main::printUsage() {
	cout << endl << "Usage: ProjectLack.exe <config-file-path>" << endl;
}

void Main::handleError(string message) {
	cerr << "====> ERROR: " + message << endl;
	exit(EXIT_FAILURE);
}

int Main::startProgram(int argc, char **argv) {
	PRN_(1, "in startProgram");

	string configFile;
	//pierwszy argument to nazwa/sciezka do pliku z config.

	if (argc != 2) {
		Main::handleError("Too little program arguments!");
		printUsage();
		return 1;
	}

	try {
		configFile = argv[1];

		cnf = new Config(configFile);

		PRN_(1, "tworzymy VoIPModule..");
		VoIPModule* voip = new VoIPModule(cnf);

		voip->connect();
	} catch (runtime_error& e) {
		Main::handleError(e.what());
	}

	PRN("TODO: zapisuje loga do pliku...");

	PRN_(1, "ends startProgram");
	return EXIT_SUCCESS;
}
