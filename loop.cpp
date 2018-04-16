#include "inputdata.h"
#include "game.h"
#include "NetworkClient.h"

void LoadGraphics(Game* thegame);
bool HandleMessages();
void SetupMenu(Game* thegame);
void Input(INPUTDATA* InputData);
void Render(Game* thegame);
void RunMenuFrame(Game* thegame, INPUTDATA* InputData, NetworkClient* Client);
void RunFrame(Game* thegame, INPUTDATA* InputData, NetworkClient* Client);

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
	//connecting loop
	thegame.msg = "waiting to connect with " + string(ServerAddress);
	while(!Client.Connected() && !InputData.Esc)
	{
		Input(&InputData);
		if(GetTickCount() > (timer + 1000))
		{
			//output message connecting to server
			Client.Knock();
			timer = GetTickCount();
		}
		Render(&thegame);
	}
	thegame.msg = "connected, waiting for other players";
	while(!InputData.Esc)
	{
		Input(&InputData);
		if(Client.messages.size())
		{
			if((*Client.messages.begin()) == "_menu")
			{
				Client.messages.pop_front();
				break;
			}
			else
			{
				thegame.msg = (*Client.messages.begin());
				Client.messages.pop_front();
			}
		}
		Render(&thegame);
	}
	thegame.msg = "Pick your color";
	SetupMenu(&thegame);			//sets up a list of colored tiles for the user to pick their color
	while(!thegame.setup && !thegame.over && HandleMessages())
    {
        Input(&InputData);
        RunMenuFrame(&thegame, &InputData, &Client);
        Render(&thegame);
    }

	//main game loop
    while(HandleMessages() && !thegame.over)
    {
        Input(&InputData);
        RunFrame(&thegame, &InputData, &Client);
        Render(&thegame);
    }
	thegame.msg = "Waiting for goodbye from server.";
	Render(&thegame);
}