/*
 * Main.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "Main.h"
#include "../debug/debug.h"
#include "../VoIPModule/VoIPModule.h"

Main* Main::main = 0;

Main::Main() {
	if (Main::main != NULL)
		return;
	main = this;
}

Main::~Main() {
	main = NULL;
}

Main* Main::getMain() {
	if (main != NULL)
		return main;
	main = new Main();
	return main;
}

void Main::printUsage() {
	cout << endl << "Usage: ProjectLack.exe <config-file-path>" << endl;
}

void Main::handleError(string message) {
	cerr << "====> ERROR: " + message << endl;
	exit(EXIT_SUCCESS);
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

		//TODO:
		PRN_(1, "tworzymy VoIPModule..");
		VoIPModule* voip = new VoIPModule(cnf);

//		voip->connect();
	} catch (runtime_error& e) {
		Main::handleError(e.what());
	}

	PRN("TODO: zapisuje loga do pliku...");

	PRN_(1, "ends startProgram");
	return 0;
}
