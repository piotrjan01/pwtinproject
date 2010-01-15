/*
 * Main.cpp
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#include "Main.h"
#include "../debug/debug.h"

Main::Main() {
	// TODO Auto-generated constructor stub

}

Main::~Main() {
	// TODO Auto-generated destructor stub
}


void Main::handleError(string message) {
	PRN("Error: ");
	PRN(message);
}

int Main::startProgram(int argc, char **argv) {
	PRN1("in startProgram");

	//print arguments
	for (int i=0; i<argc; i++) cout<<"arg nr "<<i<<" = "<<argv[i]<<endl;


	PRN("TODO: wczytanie konfiguracji");
	PRN("TODO: wczytuje dane audio");



	PRN("TODO: wywo�uje odpowiednie metody w VoIPModule..");

	PRN("TODO: zapisuje loga do pliku...");

	PRN1("ends startProgram");
	return 0;
}
