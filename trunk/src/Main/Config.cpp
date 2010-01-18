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
#include <map>

//Jesli w konfiguracji nie podano portu to uzywamy tego
#define DEFAULT_SIPPROXY_PORT 5060;

//Rozmiar danych w pakiecie RTP w bajtach
#define DEFAULT_RTP_PAYLOAD_SIZE 160

//gdy nie wysyłamy steganografii, pakiety sa wysylane w odstepach 20ms
#define DEFAULT_NOSTEG_RTP_DELAY 20

#define DEFAULT_QUEUE_READ_INTERVAL 10

Config::Config(string configFile) {
	PRN_(4, "in config constr");
	VAR_(4, configFile);

	map<string, string> settings;
	ifstream cf;

	cf.open(configFile.c_str());

	if (!cf.is_open())
		Main::getInstance()->handleError("Unable to open configuration file: "
				+ configFile);

	while (!cf.eof()) {
		string ln;
		getline(cf, ln);
		if (ln[0] == '#' || ln == "")
			continue;
		int eqpos = ln.find_first_of('=');
		string argname = ln.substr(0, eqpos);
		string argval = ln.substr(eqpos + 1);
		settings[argname] = argval;
		VAR_(4, argname+" = "+argval);
	}

	if (settings["calling"] == "1")
		weAreCalling = true;
	else if (settings["calling"] == "0")
		weAreCalling = false;
	else
		Main::getInstance()->handleError(
				"wrong configuration file format near \"calling\"");

	//wartosci domyslne parametrow tutaj:
	SIPProxyPort = DEFAULT_SIPPROXY_PORT;
	RTPPayloadSize = DEFAULT_RTP_PAYLOAD_SIZE;
	noStegRTPDelay = DEFAULT_NOSTEG_RTP_DELAY;
	incQueueReadInterval = DEFAULT_QUEUE_READ_INTERVAL;
	incQueueSize = 10;

	//ustawienia ktore wczytujemy zawsze:
	doSteg = (settings["do-steg"] == "1");

	//ustawienia ktore wczytujemy zawsze:
	if (doSteg) {
		maxStegInterval = atoi(settings["max-steg-interval"].c_str());
		minStegInterval = atoi(settings["min-steg-interval"].c_str());
		stegSequence = settings["steg-sequence"];
		stegDataFile = settings["steg-data-file"];
	}

	outputAudioFilePath = settings["output-audio-data-file"];
	myUser = settings["username"];
	myPass = settings["pass"];
	localIP = settings["local-ip"];
	SIPProxyIP = settings["proxy-ip"];
	SIPProxyPort = atoi(settings["proxy-port"].c_str());
	audioFilePath = settings["audio-data-file"];

	if (weAreCalling) {
		calleeID = settings["callee-username"];
	}

	PRN_(4, "end config constr");
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}
