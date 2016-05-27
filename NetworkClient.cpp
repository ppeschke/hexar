#include <fstream>
using namespace std;
#include "NetworkClient.h"

#pragma comment(lib, "Ws2_32.lib")

ofstream fout;

NetworkClient::NetworkClient(const char* serverAddress)
{
	fout.open("networkClient.log");
	strcpy_s(serverIP, serverAddress);
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
	Knock();
}

void NetworkClient::Send(const char* message)
{
	fout << "Sending: " << message << endl;
	sendto(Socket, message, 256, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

void NetworkClient::Knock()
{
	strcpy_s(sendBuffer, 256, "  (knock packet)");
	sendBuffer[0] = 1;
	sendBuffer[255] = '\0';
	Send(sendBuffer);
	//sendto(Socket, sendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

NetworkClient::~NetworkClient()
{
	running = false;
	fout.close();
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
	sockaddr_in IncomingAddress;
	while(running)
	{
		recvfrom(Socket, recvBuffer, 256, 0, (sockaddr*)&IncomingAddress, &SizeInt);
		if(ServerAddress.sin_addr.s_addr == IncomingAddress.sin_addr.s_addr)	//if it comes from the server
		{
			recvBuffer[255] = '\0';
			if(recvBuffer[0] == 0)	//server kick or leave acknowledgement
				break;
			locker.lock();
			messages.insert(messages.end(), recvBuffer);
			locker.unlock();
		}
	}
	return;
}

DWORD WINAPI RecvThread(LPVOID Whatever)
{
	((NetworkClient*)Whatever)->Listen();
	return 0;
}
