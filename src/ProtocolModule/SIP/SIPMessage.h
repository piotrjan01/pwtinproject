/*
 * SIPMessage.h
 * Reprezentacja wiadomosci SIP.
 */

#ifndef SIPMESSAGE_H
#define SIPMESSAGE_H

#include <iostream>
#include <string>
#include <vector>

#include "SIPAuthentication.h"

struct SIPMessage {

    friend std::ostream & operator<<(std::ostream & out, const SIPMessage & m);

    std::string				 rline;
    std::vector<std::string> via;
    std::vector<std::string> lines;
    std::string              body;

    SIPMessage() {}
    SIPMessage( char *buf, int len );

    std::string getField(std::string name);
    void setField(std::string name, std::string value);

    bool isReply();
    bool isRequest();

    int replyCode();
    std::string method();

    SIPAuthentication getAuthentication();
    SIPAuthentication getProxyAuthentication();
    std::string toStream();

    std::string getSdpAddress();
    std::string getSdpPort();

    std::string getCallID() {
        return getField("Call-ID");
    }

};

#endif

