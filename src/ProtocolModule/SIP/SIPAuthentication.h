/*
 * SIPAuthentication.h
 * Uwierzytelnienie w sesjach SIP.
 */

#ifndef SIPAUTHENTICATION_H
#define SIPAUTHENTICATION_H

#include <string>

struct SIPAuthentication {

    std::string realm, nonce, algorithm;
    std::string uri, method;
    std::string user, pass;

    std::string getMD5();

};

#endif
