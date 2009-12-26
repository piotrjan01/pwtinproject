/*
 * parsing.h 
 * Funkcje ulatwiajace parsowanie tekstu i wiadomosici SIP/SDP.
 */

#ifndef PARSING_H
#define PARSING_H

#include <string>
#include <sstream>
#include <vector>

// Usuwa z poczatku i konca s znaki c
std::string trim(std::string s, char c = ' ');
// Dzieli s na krotsze stringi rozdzielone znakiem c
std::vector<std::string> split(std::string s, char c);
// Zwraca tekst z s po znaku c
std::string getValue(std::string s, char c = '=');
// Zwraca tekst z sprzed znakiem c
std::string getKey(std::string s, char c = '=');

// Przeksztalca dowolny typ T z operatorem << (std::ostream &, const &T) na string.
template <typename T>
std::string toString(T i) {
	std::stringstream out;
	out << i;
	return out.str();
}

#endif
