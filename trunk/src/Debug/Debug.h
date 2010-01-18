/**
 * \~
 * @file Debug.h
 * \~polish
 * Zbiór makr i klasa używane do debugowania/logowania.
 * \~english
 * Macros and a class used for debugging/logging.
 * \~
 * @date created 2010-01-14
 * @author Piotr Gwizdała
 */
#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
#define PRN_(LVL, X) if (Debug::debugLevel >= LVL) { PRN(X) }
#define VAR_(LVL, X) if (Debug::debugLevel >= LVL) { VAR(X) }
#define PRNBITS_(LVL, X) if (Debug::debugLevel >= LVL) { PRNBITS(X) }

#define PRN(X) Debug::prnTime(); Debug::prn("msg:: "); Debug::prn(__FILE__); Debug::prn("@"); Debug::prn(__LINE__); \
				Debug::prn(" "); Debug::prn(X); Debug::prn("\n");
#define VAR(X) Debug::prnTime(); Debug::prn("var::"); Debug::prn(__FILE__); Debug::prn("@"); Debug::prn(__LINE__); \
				Debug::prn(": "); Debug::prn(#X); Debug::prn(" = "); Debug::prn(X); Debug::prn("\n");
#define PRNBITS(X) Debug::printBits(X);

#else
#define PRN_(LVL, X)
#define VAR_(LVL, X)
#define PRN(X)
#define VAR(X)
#define PRNBITS(X)
#define PRNBITS_(X, Y)
#endif

#include <string>
#include <iostream>
#include <sstream>
#include "../Util/Timer.h"

using namespace std;

/**
 * \~
 * @file Debug.h
 * \~polish
 * Klasa używana do debugowania/logowania.
 * \~english
 * Class used for debugging/logging.
 * \~
 * @date created 2010-01-14
 * @author Piotr Gwizdała
 */
class Debug {
public:

	static const int debugLevel = 2;

	static Timer t;

	static bool t_started;

	static void prnTime() {
		if ( ! t_started) {
			t_started = true;
			t.start();
		}
		printf("%ld: ", t.seeTime());
	}

	static void prn(string s) {
		cout << s << flush;
		return;
	}

	static void prn(long s) {
		stringstream ss;
		ss << s;
		prn(ss.str());
	}

	static void prn(int s) {
		stringstream ss;
		ss << s;
		prn(ss.str());
	}

	static void prn(char s) {
		stringstream ss;
		ss << s;
		prn(ss.str());
	}

	static void printBits(const char* s, int len) {
		int i, j, v;
		for (i = 0; i < len; ++i) {
			v = s[i];
			cout << "[" << i << "]=<";
			for (j = 0; j < 8; ++j) {
				cout << (((v >> (7 - j)) & 0x01) ? 1 : 0);
			}
			cout << "> ";
		}
		cout << endl << flush;
	}
	static void printBits(string s) {
		printBits(s.data(), s.length());
	}
};


#endif /* DEBUG_H_ */
