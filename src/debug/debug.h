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

#define PRN(X) Dbg::prn("msg: "); Dbg::prn(__FILE__); Dbg::prn("@"); Dbg::prn(__LINE__); Dbg::prn(" "); Dbg::prn(X); Dbg::prn("\n");
#define VAR(X) Dbg::prn("var: "); Dbg::prn(#X); Dbg::prn(" = "); Dbg::prn(X); Dbg::prn("\n");

#else
#define PRN_(LVL, X)
#define VAR_(LVL, X)
#define PRN(X)
#define VAR(X)
#endif

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Dbg {
public:

	static const int debugLevel = 2;

	static void prn(string s) {
		cout << s << flush;
		return;
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

};

#endif /* DEBUG_H_ */
