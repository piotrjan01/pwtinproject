
sip: main.o sip_agent.o sip_message.o sip_authentication.o parsing.o
	g++ main.o sip_agent.o sip_message.o sip_authentication.o parsing.o -lstdc++ -lpthread -lssl -o sip

main.o: main.cpp sip_agent.h
	g++ -c -Wall -g main.cpp 

sip_agent.o: sip_agent.cpp sip_agent.h sip_message.h sip_authentication.h parsing.h semaphore.h
	g++ -c -Wall -g sip_agent.cpp

sip_message.o: sip_message.cpp sip_message.h sip_authentication.h parsing.h
	g++ -c -Wall -g sip_message.cpp

parsing.o: parsing.cpp parsing.h
	g++ -c -Wall -g parsing.cpp

sip_authentication.o: sip_authentication.cpp sip_authentication.h
	g++ -c -Wall -g sip_authentication.cpp

clean:
	rm *.o
	rm sip

