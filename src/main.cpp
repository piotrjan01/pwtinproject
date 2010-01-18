/**
 * \~english
 * @mainpage [TIN] ProjectLACK
 *
 * @section sIntroduction Introduction
 * The idea of the project...
 *
 * @section sAuthors Authors
 * \~
 * \li Piotr Gwizdała
 *
 * \li Bogdan Jezierski
 *
 * \li Marek Kurdej
 *
 * \li Michał Leśniewski
 */

#include "Main/Main.h"

using namespace std;

/**
 * FIXME: Ogólna lista zyczen:
 *
 * todo: wczytywanie konfiga w pelni
 * todo: poziom logowania ustawic lepiej (korzystajac z Debug.h) i do skonfigurowania w pliku
 *
 * todo: wysylanie pakietow bez steg tez mogloby przyjmowac losowe opoznienia z podanego przedzialu... ?
 *
 */

int main(int argc, char **argv) {
	/* Jedyne co robimy to przekazujemy argumenty do metody
	 * startProgram w klasie Main i dzia�amy ju� tylko na obiektach.
	 */
	return Main::getInstance()->startProgram(argc, argv);
}

