/*
 * Config.cpp
 *
 *  Created on: Jan 15, 2010
 *      Author: piotrrr
 */

#include "Config.h"
#include "../debug/debug.h"
#include "Main.h"
#include <iostream>
#include <fstream>

Config::Config(string configFile) {
	VAR_(2, configFile);
	ifstream cf;
	string callIndicator;
	cf.open(configFile.c_str());
	if (cf.is_open()) {
		getline(cf, callIndicator);
		if (callIndicator == "calling") {
			weAreCalling = true; VAR_(2, weAreCalling);
			getline(cf, myUser); VAR_(2, myUser);
			getline(cf, myPass); VAR_(2, myPass);
			getline(cf, localIP); VAR_(2, localIP);
			getline(cf, SIPProxyIP); VAR_(2, SIPProxyIP);
			getline(cf, calleeID); VAR_(2, calleeID);
			getline(cf, audioFilePath); VAR_(2, audioFilePath);
		}
		else if (callIndicator == "answering") {
			weAreCalling = false; VAR_(2, weAreCalling);
			getline(cf, myUser); VAR_(2, myUser);
			getline(cf, myPass); VAR_(2, myPass);
			getline(cf, localIP); VAR_(2, localIP);
			getline(cf, SIPProxyIP); VAR_(2, SIPProxyIP);
			getline(cf, audioFilePath); VAR_(2, audioFilePath);
			getline(cf, outputAudioFilePath); VAR_(2, outputAudioFilePath);
		} else Main::getMain()->handleError("Configuration file has incorrect format.");
		//cout << line << endl;
		cf.close();
	} else {
		Main::getMain()->handleError("Unable to open configuration file: "+ configFile);
	}

	//TODO: usunac i wczytac z pliku
	doSteg = false;
	PRN_(2, "end config constr");
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}
