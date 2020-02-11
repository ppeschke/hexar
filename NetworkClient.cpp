#include <fstream>
#include "NetworkClient.h"
#include <Ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define TIMEOUT_THRESHOLD_MS 60000

ofstream fout;

float updateTime();

NetworkClient::NetworkClient(const char* serverAddress)
{
	strcpy_s(serverIP, serverAddress);
	port = 17000;
	fout.open("networkClient.log", ios::out);
	fout << "Initializing WinSock..." << endl;
	if (WSAStartup(MAKEWORD(2, 2), &Winsock) != 0)
	{
		fout << "Winsock intialization failed: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}
	fout << "Initialized." << endl;
	
	if (SOCKET_ERROR == (Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
	{
		fout << "Failed to create socket: " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_port = htons(port);
	if (InetPton(AF_INET, serverIP, &ServerAddress.sin_addr.S_un.S_addr) != 1)
	{
		fout << "Failed to create inet address." << endl;
		closesocket(Socket);
		WSACleanup();
		return;
	}
	
	running = true;

	unsigned int tries = 0;
	connected = false;
	while (!connected && tries < 7)
	{
		Knock();
		++tries;
		if (!connected)
		{
			Sleep(3000);
		}
	}
	ListenThreadHandle = CreateThread(NULL, 0, RecvThread, this, 0, NULL);
}

NetworkClient::~NetworkClient()
{
	running = false;
	Leave();
	fout.close();
	WaitForSingleObject(ListenThreadHandle, INFINITE);	//wait for the listen thread to finish
	closesocket(Socket);
	WSACleanup();
}

void NetworkClient::Knock()
{
	if (running)
	{
		if (connect(Socket, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress)) != 0)
		{
			fout << "Failed to connect: " << WSAGetLastError() << endl;
		}
		else
		{
			connected = true;
		}
	}
}

void NetworkClient::Send(const char* message)
{
	fout << "Sending: " << message << endl;
	strcpy_s(sendBuffer, message);
	sendBuffer[255] = '\0';
	sendto(Socket, sendBuffer, 256, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

bool NetworkClient::Connected()
{
	return connected;
}

void NetworkClient::Leave()
{
	sendBuffer[0] = '\0';
	sendto(Socket, sendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
	connected = false;
}

void NetworkClient::Listen()
{
	ofstream listenOut("ListenThread.log");
	int bytesReceived;

	while(running)
	{
		switch(RecvWithTimeout(5))
		{
		case (0):	//timeout, check if we're still running in the main thread
			continue;
		case (-1):
			DWORD error;
			error = GetLastError();
			running = false;
			break;
		default:
			bytesReceived = recv(Socket, recvBuffer, 256, 0);
			if (bytesReceived == -1)
			{
				error = GetLastError();
				if (error == WSAECONNRESET)
				{
					break;
					continue;
				}
			}
			else if (bytesReceived == 0)
			{
				//server disconnected
				closesocket(Socket);
				Socket = 0;
				connected = false;
				running = false;
			}
			else
			{
				recvBuffer[255] = '\0';
				fout << recvBuffer << endl;
				if (recvBuffer[0] == 0)	//server kick or leave acknowledgement
				{
					connected = false;
					running = false;
				}
				locker.lock();
				messages.insert(messages.end(), recvBuffer);
				locker.unlock();
			}
			break;
		}
	}
	running = false;
	listenOut.close();
	return;
}

int NetworkClient::RecvWithTimeout(long sec)
{
	// Setup timeval variable
	timeval timeout;
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;
	// Setup fd_set structure
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(Socket, &fds);
	// Return value:
	//  -1: error occurred
	//   0: timed out
	// > 0: data ready to be read
	return select(NULL, &fds, NULL, NULL, &timeout);
}

DWORD WINAPI RecvThread(LPVOID Whatever)
{
	((NetworkClient*)Whatever)->Listen();
	return 0;
}
