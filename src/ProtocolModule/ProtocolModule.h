/*
 * ProtocolModule.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda³a
 */

#ifndef PROTOCOLMODULE_H_
#define PROTOCOLMODULE_H_

class ProtocolModule {
public:
	ProtocolModule();
	virtual ~ProtocolModule();

	/**
	 * Laczy sie ze wskazanym serwerem
	 */
	bool connect(int IP, int port, int proxy);

	/**
	 * Wysyla podany pakiet
	 */
	void send(char** data);

	/**
	 * Pobiera rozmiar pakietu wynegocjowany w polaczeniu... ?
	 */
	int getPackageSize();

	/**
	 * Rozlacza sie
	 */
	void disconnect();

	/**
	 * Odbiera dane do retBuffer z po³aczenia
	 */
	void receive(char** retBuff);

};

#endif /* PROTOCOLMODULE_H_ */
