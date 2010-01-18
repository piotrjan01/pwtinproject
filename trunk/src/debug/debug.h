/*
 * debug.h
 *
 *  Created on: 2010-01-14
 *      Author: Piotr Gwizda�a
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#ifdef DEBUG
#define PRN_(LVL, X) if (Dbg::debugLevel >= LVL) { PRN(X) }
#define VAR_(LVL, X) if (Dbg::debugLevel >= LVL) { VAR(X) }
#define PRNBITS_(LVL, X) if (Dbg::debugLevel >= LVL) { PRNBITS(X) }

#define PRN(X) Dbg::prnTime(); Dbg::prn("msg:: "); Dbg::prn(__FILE__); Dbg::prn("@"); Dbg::prn(__LINE__); \
				Dbg::prn(" "); Dbg::prn(X); Dbg::prn("\n");
#define VAR(X) Dbg::prnTime(); Dbg::prn("var::"); Dbg::prn(__FILE__); Dbg::prn("@"); Dbg::prn(__LINE__); \
				Dbg::prn(": "); Dbg::prn(#X); Dbg::prn(" = "); Dbg::prn(X); Dbg::prn("\n");
#define PRNBITS(X) Dbg::printBits(X);

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

class Dbg {
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
