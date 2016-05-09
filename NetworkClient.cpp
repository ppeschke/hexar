#include "NetworkClient.h"

#pragma comment(lib, "Ws2_32.lib")

NetworkClient::NetworkClient()
{
	strcpy_s(serverIP, "127.0.0.1");
	port = 17000;
	SizeInt = sizeof(ServerAddress);
	WSAStartup(MAKEWORD(2, 2), &Winsock);
	running = true;
	if(LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)
	{
		WSACleanup();
		exit(EXIT_FAILURE);
	}

	Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	ZeroMemory(&ServerAddress, SizeInt);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(serverIP);
	ServerAddress.sin_port = port;

	ListenThreadHandle = CreateThread(NULL, 0, RecvThread, this, 0, NULL);
}

void NetworkClient::Send(const char* message)
{
	sendto(Socket, message, 256, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

void NetworkClient::Knock()
{
	sendBuffer[0] = 1;
	sendto(Socket, sendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

NetworkClient::~NetworkClient()
{
	running = false;
	WaitForSingleObject(ListenThreadHandle, INFINITE);	//wait for the listen thread to finish
	WSACleanup();
}

void NetworkClient::Leave()
{
	sendBuffer[0] = 0;
	sendto(Socket, sendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
	running = false;
}

void NetworkClient::Listen()
{
	while(running)
	{
		recvfrom(Socket, recvBuffer, 256, 0, (sockaddr*)&ServerAddress, &SizeInt);
		recvBuffer[255] = '\0';
		if(recvBuffer[0] == 0)	//server kick or leave acknowledgement
			break;
		locker.lock();
		messages.insert(messages.end(), recvBuffer);
		locker.unlock();
	}
	return;
}

DWORD WINAPI RecvThread(LPVOID Whatever)
{
	((NetworkClient*)Whatever)->Listen();
	return 0;
}
