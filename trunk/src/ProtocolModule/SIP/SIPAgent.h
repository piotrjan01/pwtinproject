/**
 * @file SIPAgent.h
 * Agent SIP - główna klasa
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

    /**
     * Konstruktor. Parametr to lokalny adres IP, który ma być wykorzystany.
     */
    SIPAgent(std::string localaddr);
    ~SIPAgent();

    /**
     * Rejestruje użytkownika na serwerze SIP Registrar. Parametry to nazwa użytkownika, hasło,
     * adres serwera proxy oraz port proxy.
     * Funkcja blokuje do chwili zarejestrowania lub upłynięcia 30 sekund. Po udanej rejestracji
     * zwraca true.
     */
    bool Register(std::string user, std::string pass, std::string proxy, unsigned short proxyPort);

    /**
     * Wyrejestrowuje użytkownika z serwera SIP Registrar. Funkcja blokuje do chwili potwierdzenia.
     * Błędy oraz timeout 30 sekund jest ignorowany
     */
    void Unregister();

    /**
     * Oczekuje na przychodzące połączenie i odbiera je. Funkcja blokuje, jednak nie dłużej niż 120
     * sekund. Jako parametr podany jest numer portu, na którym działa RTP. Funkcja zwraca strukturę
     * CallInfo z informacjami o użytkowniku, z którym połączono - identyfikator, numer ip oraz port
     * RTP. W przypadku błędu w strukturze CallInfo adres ma wartość "0.0.0.0".
     */
    CallInfo Answer(unsigned short port);

    /**
     * Nawiązuje połączenie z użytkownikiem o wskazanym identyfikatorze. Drugi parametr to numer portu
     * RTP. Funkcja blouje, jednak nie dłużej niż 30 sekund. Funkcja zwraca strukturę CallInfo z
     * informacjami o użytkowniku, z którym połączono - identyfikator, numer ip oraz port RTP.
     * W przypadku błędu w strukturze CallInfo adres ma wartość "0.0.0.0".
     */
    CallInfo Call(std::string id, unsigned short port);

    /**
     * Rozłącza nawiązane połączone.
     */
    void Disconnect();

    /**
     * Sprawdza, czy nadal trwa połączenie z drugim użytkownikiem. Zwraca false, gdy połączenie zostało
     * rozłączone (przez którąś ze stron).
     */
    bool connected();

};
#endif

