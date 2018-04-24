#include "inputdata.h"
#include "game.h"
#include "NetworkClient.h"

void LoadGraphics(Game* thegame);
bool HandleMessages();
void SetupMenu(Game* thegame);
void Input(INPUTDATA* InputData);
void Render(Game* thegame);
void RunMenuFrame(Game* thegame, INPUTDATA* InputData, NetworkClient* Client, DWORD deltaTime);
void RunFrame(Game* thegame, INPUTDATA* InputData, NetworkClient* Client, DWORD deltaTime);

// The Outer Loop
void OuterLoop(const char* ServerAddress)
{
	DWORD timer;
	Game thegame;
	INPUTDATA InputData;
	NetworkClient Client(ServerAddress);
	LoadGraphics(&thegame);
	Input(&InputData);
	timer = GetTickCount();
	DWORD startingPoint = GetTickCount();
	DWORD deltaTime;
	//connecting loop
	thegame.messages.AddMessage("connecting with " + string(ServerAddress), 10);
	while(!Client.Connected() && !InputData.Esc)
	{
		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();
		Input(&InputData);
		thegame.messages.Run(deltaTime);
		if(GetTickCount() > (timer + 1000))
		{
			//output message connecting to server
			Client.Knock();
			timer = GetTickCount();
		}
		thegame.messages.Run(deltaTime);
		Render(&thegame);
	}
	thegame.messages.AddMessage("connected, waiting for other players", 10);
	startingPoint = GetTickCount();
	while(true)
	{
		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();
		Input(&InputData);
		thegame.messages.Run(deltaTime);
		if(Client.messages.size())
		{
			if((*Client.messages.begin()) == "_menu")
			{
				Client.messages.pop_front();
				break;
			}
			else
			{
				thegame.messages.AddMessage((*Client.messages.begin()), 10);
				Client.messages.pop_front();
			}
		}
		Render(&thegame);
	}
	thegame.messages.AddMessage("Pick your color", 10);
	SetupMenu(&thegame);			//sets up a list of colored tiles for the user to pick their color
	startingPoint = GetTickCount();
	while(!thegame.setup && !thegame.over && HandleMessages())
    {
		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();
        Input(&InputData);
        RunMenuFrame(&thegame, &InputData, &Client, deltaTime);
        Render(&thegame);
    }

	startingPoint = GetTickCount();
	thegame.messages.AddMessage("Test\nMessage", 10);
	//main game loop
    while(HandleMessages() && !thegame.over)
    {
		deltaTime = GetTickCount() - startingPoint;
		startingPoint = GetTickCount();
        Input(&InputData);
        RunFrame(&thegame, &InputData, &Client, deltaTime);
        Render(&thegame);
    }
	thegame.messages.AddMessage("Waiting for goodbye from server.", 10);
	Render(&thegame);
}