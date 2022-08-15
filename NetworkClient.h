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
	int RecvWithTimeout(long sec);
	void Knock();
	void Send(const char* message);
	bool Connected();
	void Leave();

	mutex			locker;
	HANDLE			ListenThreadHandle;
	wchar_t			serverIP[17];
	unsigned int	port;
	WSADATA			Winsock;
	SOCKET			Socket;
	sockaddr_in		ServerAddress;
	char			recvBuffer[256];
	char			sendBuffer[256];
	list<string>	messages;
	volatile bool	running;
	volatile bool	connected;
};

#endif