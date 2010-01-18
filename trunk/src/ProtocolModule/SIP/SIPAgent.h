/*
 * SIPAgent.h
 * Agent SIP - glowna klasa
 */

#ifndef SIPAGENT_H
#define SIPAGENT_H

#include <string>
#include <sstream>
#include <vector>

#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../../Debug/Debug.h"

#include "../../Util/Semaphore.h"
#include "../../Util/Net.h"

#include "SIPMessage.h"
#include "SIPAuthentication.h"

struct CallInfo {
    std::string callID;
    std::string remoteIP;
    unsigned short remotePort;
};

struct SIPInfo {
    unsigned int cseq;
    std::string callid;
    SIPAuthentication authentication;
};

enum WaitFor { NONE, REGISTER, CALL, ANSWER };

class SIPAgent {

    friend void * SIPAgentThread( void * );
    friend class SIPMessage;

private:

    double sleepTime;

    // Threading
    pthread_t thread;
    Semaphore mutex;

    // Semafor pozwalajacy na blokowanie metod
    Semaphore wait;
    WaitFor waitingfor;
    // Do timeoutow
    unsigned long waitTime;

    // Socket
    int sock;

    // Adres lokalny
    sockaddr_in address;

    // Ustawione na true zabija
    bool killThread;
    bool registered;

    // Podstawowe informacje o uzytkowniku
    std::string proxy, user, pass, tag, branch;
    std::string fromtoline;

    // Sesja rejestrujaca
    SIPInfo registration;

    // Sesja nawiazujaca polaczenia
    SIPInfo invite;

    // port proxy
    unsigned short proxyPort;

    // porty RTP
    unsigned short localRtpPort, remoteRtpPort;

    // identyfikator oraz ip rozm�wcy
    std::string partnerID, remoteRtpAddress, partnerTag;
    bool connectionEstablished;


    std::string addressString();

    std::string generateBranch();
    std::string generateCallID();
    void generateFromTag();

	bool unregistering;

    void sendMessage(SIPMessage &m, std::string address, unsigned short port);
    void receiveMessage();

    void replyAck(SIPMessage &m, std::string information = "");
    void replyToOptions(SIPMessage &m);
    void replyRinging(SIPMessage &m);

    void answerCall(SIPMessage &m);

    void Register();
    void Call();


public:

    SIPAgent(std::string localaddr);
    ~SIPAgent();

    bool Register(std::string user, std::string pass, std::string proxy, unsigned short proxyPort);

    void Unregister();

    CallInfo Answer(unsigned short port);
    CallInfo Call(std::string id, unsigned short port);

    void Disconnect();

    bool connected();

};
#endif

