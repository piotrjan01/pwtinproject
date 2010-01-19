/**
 *\~
 *
 *\~polish
 * @mainpage [TIN] ProjectLACK
 *
 * @section sWstep Wstep
 * Celem projektu była implementacja prostej aplikacji konsolowej zgodnej
 * z prostymi protokołami sieciowymi VoIP (telefonii internetowej)
 * z mechanizmem steganograficznym wykorzystującym metodę LACK.
 *
 * Zgodnie z koncepcją steganografii, metoda LACK pozwala wraz ze strumieniem danych audio
 * przesyłać dodatkowe, ukryte informacje nie wpływając na jakość przesyłanego dźwięku.
 * Metoda nie generuje również dodatkowego ruchu w sieci, co utrudnia wykrycie faktu
 * ukrytej komunikacji.
 *
 * Kluczowym elementem metody jest dobranie opóźnień sekwencji pakietów
 * z danymi audio w taki sposób, by mieć pewność, że dany pakiet zostanie
 * odrzucony przez klienta nieświadomego komunikacji steganograficznej.
 * Projektowana aplikacja ma umożliwić eksperymentalne dobranie parametrów transmisji,
 * aby zbadać skuteczność metody LACK.
 *
 * @section sAutorzy Autorzy
 * \~english
 * @mainpage [TIN] ProjectLACK
 *
 * @section sIntroduction Introduction
 * The aim of the project was to implement a simple console application conforming to
 * VoIP (voice-over-IP) network protocols with a steganographic mechanism that uses
 * the LACK method.
 *
 * According to the concept of steganography, the LACK method let us send audio data
 * together with additional, hidden information without any noticeable loss of quality.
 * Our method doesn't generate additional network traffic, which makes it harder to
 * detect the hidden communication.
 *
 * The key element of the method is choosing appropriate values of delays for packet sequence
 * with audio data in order to be sure, that steganographic packets will be dropped unnoticed
 * by a client unaware of the method.
 * The project is designed to let us choose experimentally appropriate parameters of the
 * transmission in order to test the efficiency of the method.
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

