#include <cstdlib>
#include <iostream>
#include "sip_agent.h"
#include "sip_message.h"

using namespace std;

//by Piotr: definiuje konfigi, zeby mozna bylo testowac
//w roznych miejscach.
#define CNF_PIOTR
//#define CNF_MICHAL

#if defined(CNF_PIOTR)
	#define MY_IP "192.168.0.103"
	#define MY_UID "stud6"
	#define MY_PASS "stud6"
	#define PROXY_IP "192.168.0.100"
	#define PROXY_PORT 5060
	#define UID_TO_CALL "piotrrr"
	#define PORT_TO_CALL 3333

#elif defined(CNF_MICHAL)
	#define MY_IP "192.168.0.212"
	#define MY_UID "stud6"
	#define MY_PASS "stud6"
	#define PROXY_IP "194.29.169.4"
	#define PROXY_PORT 8060
	#define UID_TO_CALL "stud5"
	#define PORT_TO_CALL 3333
#endif



int main(int argc, char *argv[]) {

    srand( time(NULL) );

    SIP_Agent agent(MY_IP);

    agent.Register(MY_UID, MY_PASS, PROXY_IP, PROXY_PORT);

    agent.Call(UID_TO_CALL, PORT_TO_CALL);

//	cout << "MAIN \n";

    sleep(5.0);

    agent.Disconnect();

    sleep(5.0);

    return 0;

}


