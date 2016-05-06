#ifndef CLIENT_H
#define CLIENT_H

#include <string>
using namespace std;

class client
{
public:
	client(const char*, unsigned int);
	~client(void);

	string server_address;
	unsigned int server_port;

	void connect();			//knocks
	void listen();			//listens for incoming messages
							//stop listening is automatic
	void disconnect();		//announces goodbye
	void send(const char*);	//sends a message
	bool hasMessage();
	string getAMessage();

	bool connected;
	bool listening;
};

#endif