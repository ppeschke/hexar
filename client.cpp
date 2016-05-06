#define _WINSOCKAPI_    // Don't include Winsock.h
#include <winsock2.h>    // WinSock header file
#include <queue>
#include <fstream>
using namespace std;

#pragma comment(lib, "Ws2_32.lib")
#include "client.h"

ofstream fout;
DWORD WINAPI RecvThread(LPVOID Whatever);    // The RecvThread() prototype

WSADATA Winsock;			// Stores information about Winsock
SOCKET Socket;				// The ID of the socket
sockaddr_in ServerAddress;	// The address to send data to
char SendBuffer[64];	    // The buffer of data to send
char RecvBuffer[64];		// The buffer of data to receive
int SizeInt;
bool recvd = false;
bool listening, connected;
queue<const char*> messages;

client::client(const char* sa, unsigned int sp)
{
	fout.open("recv.txt");
	server_address = sa;
	server_port = sp;
	SizeInt = sizeof(ServerAddress);
	WSAStartup(MAKEWORD(2, 2), &Winsock);

    if(LOBYTE(Winsock.wVersion) != 2 || HIBYTE(Winsock.wVersion) != 2)		//Check version
    {
        WSACleanup();
        return;
    }
	//Make the Socket
    Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Input Server Information
    ZeroMemory(&ServerAddress, sizeof(ServerAddress));			//clear the struct
    ServerAddress.sin_family = AF_INET;							//set the address family
    ServerAddress.sin_addr.s_addr = inet_addr(server_address.c_str());	//set the IP address
    ServerAddress.sin_port = server_port;						//set the port
	listening = false;
	connected = false;
}

client::~client(void)
{
	if(connected)
		disconnect();
	WSACleanup();
	fout.close();
}

void client::connect()
{
	//Send a knock to the server
    SendBuffer[0] = 1;
    sendto(Socket, SendBuffer, 1, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
	connected = true;
}

void client::listen()
{
	char temp[2] = {1, 0};
	messages.push(temp);
	messages.push("_granted yellow");
	messages.push("_taken yellow");
	messages.push("_setup 2 4");
	messages.push("_grab 0 0 4");
	//Start the receiver thread
    CreateThread(NULL, 0, RecvThread, NULL, 0, NULL);
	listening = true;
}

void client::disconnect()
{
	connected = false;
	SendBuffer[0] = 0;
	send(SendBuffer);
}

void client::send(const char* message)
{
	sendto(Socket, message, 64, 0, (sockaddr*)&ServerAddress, sizeof(sockaddr));
}

bool client::hasMessage()
{
	return !messages.empty();
}

string client::getAMessage()
{
	string m = "";
	if(hasMessage())
	{
		m = messages.front();
		messages.pop();
	}
	return m;
}

DWORD WINAPI RecvThread(LPVOID Whatever)
{
    while(true)
    {
        //Wait for messages to arrive
        recvfrom(Socket, RecvBuffer, 64, 0, (sockaddr*)&ServerAddress, &SizeInt);

        //If the server kicks or acknowledges a disconnect, goodbye
        if(RecvBuffer[0] == 0)
            connected = false;
		else if(RecvBuffer[0] == 1)
			connected = true;
		else
		{
			messages.push(RecvBuffer);
			fout << RecvBuffer << endl;
		}
    }
    return 0;
}