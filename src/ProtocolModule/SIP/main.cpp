#include <cstdlib>
#include <iostream>
#include "sip_agent.h"
#include "sip_message.h"

using namespace std;

int main(int argc, char *argv[]) {

	srand(time(NULL));

	SIP_Agent agent("192.168.0.212");

	agent.Register("stud6", "stud6", "194.29.169.4");

	cout << " --------------- MAIN -------------- " << endl;

	agent.Call("stud6", 3333);

	sleep(10.0);
	return 0;

}

