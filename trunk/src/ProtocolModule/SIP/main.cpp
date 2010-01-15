
#include <cstdlib>
#include <iostream>
/*#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> 
#include <string>
#include <vector>*/

#include "sip_agent.h"
#include "sip_message.h"

using namespace std;

int main(int argc, char *argv[]) {

	srand(time(NULL));

	SIP_Agent agent("192.168.169.9");

	agent.Register("stud6", "stud6", "194.29.169.4");

	sleep(100.0);
	return 0;

}
