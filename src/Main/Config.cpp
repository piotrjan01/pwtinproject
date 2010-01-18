/*
 * Config.cpp
 *
 *  Created on: Jan 15, 2010
 *      Author: piotrrr
 */

#include "Config.h"
#include "../Debug/Debug.h"
#include "Main.h"
#include <iostream>
#include <fstream>
#include <map>

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

	doSteg = (settings["do-steg"] == "1");

	if (doSteg) {
		maxStegInterval = atoi(settings["max-steg-interval"].c_str());
		minStegInterval = atoi(settings["min-steg-interval"].c_str());
		stegSequence = settings["steg-sequence"];
		stegDataFile = settings["steg-data-file"];
		outputStegDataFile = settings["output-steg-data-file"];
	}

	outputAudioFilePath = settings["output-audio-data-file"];
	myUser = settings["username"];
	myPass = settings["pass"];
	localIP = settings["local-ip"];
	SIPProxyIP = settings["proxy-ip"];
	SIPProxyPort = atoi(settings["proxy-port"].c_str());
	audioFilePath = settings["audio-data-file"];

	SIPProxyPort = atoi(settings["sip-proxy-port"].c_str());
	RTPPayloadSize = atoi(settings["rtp-payload-size"].c_str());
	noStegRTPDelay = atoi(settings["no-steg-rtp-delay"].c_str());
	incQueueReadInterval = atoi(settings["inc-queue-read-interval"].c_str());
	incQueueSize = atoi(settings["inc-queue-size"].c_str());

	if (weAreCalling) {
		calleeID = settings["callee-username"];
	}

	PRN_(4, "end config constr");
}

Config::~Config() {
	// TODO Auto-generated destructor stub
}
