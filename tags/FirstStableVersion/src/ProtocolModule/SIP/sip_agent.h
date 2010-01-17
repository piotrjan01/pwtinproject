/*
 * sip_agent.h
 * Agent SIP - glowna klasa
 */

#ifndef SIP_AGENT_H
#define SIP_AGENT_H

#include <string>
#include <sstream>
#include <vector>

#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "semaphore.h"

#include "sip_message.h"
#include "sip_authentication.h"

struct CallInfo {
    std::string callID;
    std::string remoteIP;
    unsigned short remotePort;
};

struct SIP_Info {
    unsigned int cseq;
    std::string callid;
    SIP_Authentication authentication;
};

enum Waitfor { NONE, REGISTER, CALL, ANSWER };

class SIP_Agent {

    friend void * SIP_AgentThread( void * );
    friend class SIP_Message;

private:

    double sleepTime;

    // Threading
    pthread_t thread;
    Semaphore mutex;

    // Semafor pozwalajacy na blokowanie metod
    Semaphore wait;
    Waitfor waitingfor;
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
    SIP_Info registration;

    // Sesja nawiazujaca polaczenia
    SIP_Info invite;

    // port proxy
    unsigned short proxyPort;

    // porty RTP
    unsigned short localRtpPort, remoteRtpPort;

    // identyfikator oraz ip rozmówcy
    std::string partnerID, remoteRtpAddress, partnerTag;
    bool connectionEstablished;


    std::string addressString();

    std::string generateBranch();
    std::string generateCallID();
    void generateFromTag();

	bool unregistering;

    void sendMessage(SIP_Message &m, std::string address, unsigned short port);
    void receiveMessage();

    void replyAck(SIP_Message &m, std::string information = "");
    void replyToOptions(SIP_Message &m);
    void replyRinging(SIP_Message &m);

    void answerCall(SIP_Message &m);

    void Register();
    void Call();


public:

    SIP_Agent(std::string localaddr);
    ~SIP_Agent();

    bool Register(std::string user, std::string pass, std::string proxy, unsigned short proxyPort);

    void Unregister();

    CallInfo Answer(unsigned short port);
    CallInfo Call(std::string id, unsigned short port);

    void Disconnect();

    bool connected();

};
#endif

