#include <cstdlib>
#include <iostream>
#include "sip_agent.h"
#include "sip_message.h"

using namespace std;


int main(int argc, char *argv[]) {

	srand(time(NULL));

	SIP_Agent agent("192.168.0.212");

	agent.Register("stud6", "stud6", "194.29.169.4", 8060);

	agent.Call("stud5", 3333);

//	cout << "MAIN \n";

	sleep(5.0);

	agent.Disconnect();

	sleep(5.0);

	return 0;

}


