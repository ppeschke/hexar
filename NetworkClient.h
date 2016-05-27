#ifndef NETWORK_CLIENT
#define NETWORK_CLIENT

#define _WINSOCKAPI_
#include <WinSock2.h>
#include <list>
#include <mutex>
using namespace std;

DWORD WINAPI RecvThread(LPVOID Whatever);

class NetworkClient
{
public:
	NetworkClient(const char* serverAddress);
	~NetworkClient();

	void Listen();
	void Knock();
	void Send(const char* message);
	void Leave();

	mutex locker;
	HANDLE ListenThreadHandle;
	char serverIP[17];
	unsigned int port;
	WSADATA Winsock;
	SOCKET Socket;
	sockaddr_in ServerAddress;
	int SizeInt;
	char recvBuffer[256];
	char sendBuffer[256];
	volatile bool running;
	list<char*> messages;
};

#endif