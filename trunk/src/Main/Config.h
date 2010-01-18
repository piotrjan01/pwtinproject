/**
 * \~
 * @file Config.h
 * \~polish
 * Klasa obsługująca pliki konfiguracyjne aplikacji.
 * \~english
 * Class responsible for reading [writing] configuration files.
 * \~
 * @date created 2010-01-15
 * @author Piotr Gwizdała
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

using namespace std;

/**
 * \~
 * \~polish
 * Klasa obsługująca pliki konfiguracyjne aplikacji.
 * \~english
 * Class responsible for reading [writing] configuration files.
 * \~
 */
class Config {
public:
	Config(string configFilePath);
	virtual ~Config();

	/**
	 * Ustawione na true jesli to my dzwonimy. Jak nie to odbieramy polaczenie.
	 */
	bool weAreCalling;

	/**
	 * Nasz adres IP
	 */
	string localIP;

	/**
	 * Nasz user id np. stud6
	 */
	string myUser;

	/**
	 * Nasze hasło do proxy
	 */
	string myPass;

	/**
	 * IP proxy
	 */
	string SIPProxyIP;

	/**
	 * The port through which we connect with proxy
	 */
	int SIPProxyPort;

	//ID usera do ktorego dzwonimy
	string calleeID;

	/**
	 * ścieżka do pliku z dziweikiem ktory wysylamy
	 */
	string audioFilePath;

	/**
	 * ścieżka do pliku w którym zapisujemy dane audio przychodzące.
	 * tylko w przypadku odbierania połączenia
	 */
	string outputAudioFilePath;

	/**
	 * Mówi czy bedziemy przesylac dane steganograficzne czy nie
	 */
	bool doSteg;

	/*
	 *maximum interval between steg sequence
	 */
	int maxStegInterval;

	/**
	 *minimum interval between steg sequence
	 */
	int minStegInterval;

	/**
	 * Steganography sequence from config file
	 */
	string stegSequence;

	/**
	 * Path to file with secret data
	 */
	string stegDataFile;

	/**
	 * the size of RTP packet payload in bytes
	 */
	int RTPPayloadSize;

	/**
	 * opoznienie miedzy pakietami gdy wysylamy normalne dane audio z pliku
	 */
	int noStegRTPDelay;

	/**
	 * Czas pomiedzy odczytywaniem pakietow z kolejki pakietow przychodzacych
	 * (razem z rozmiarem kolejki definiuje model kolejki pakietow przychodzacych)
	 * po tym czasie, powinna zostać przeczytana cała kolejka.
	 */
	long incQueueReadInterval;

	/**
	 * Rozmiar kolejki pakietów przychodzacych
	 */
	int incQueueSize;

};

#endif /* CONFIG_H_ */
