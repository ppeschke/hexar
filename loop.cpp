#include "inputdata.h"
#include "game.h"
#include "NetworkClient.h"

void LoadGraphics(Game* thegame);
bool HandleMessages();
void SetupMenu(Game* thegame);
void Input(INPUTDATA* InputData);
void Render(Game* thegame);
void RunMenuFrame(Game* thegame, INPUTDATA* InputData, NetworkClient* Client, float seconds);
void RunFrame(Game* thegame, INPUTDATA* InputData, NetworkClient* Client, float seconds);
inline float updateTime();

// The Outer Loop
void OuterLoop(const char* ServerAddress)
{
	Game thegame;
	thegame.logFile.open("log.txt");
	INPUTDATA InputData;
	NetworkClient Client(ServerAddress);
	LoadGraphics(&thegame);
	Input(&InputData);
	float seconds = updateTime();

	//connecting loop
	thegame.messages.AddMessage("connecting with " + string(ServerAddress), 5.0f);
	float connectionAttemptTimer = seconds;
	float prev = 0.0;
	while(!Client.Connected() && !InputData.Esc)
	{
		seconds = updateTime();
		connectionAttemptTimer += seconds;
		Input(&InputData);
		thegame.messages.Run(seconds);
		if((connectionAttemptTimer - 3.0f) >= prev)	//it's been more than a second
		{
			prev = connectionAttemptTimer;
			//output message connecting to server
			thegame.messages.AddMessage("retrying connection", 3.0f);
			Client.Knock();
		}
		thegame.messages.Run(seconds);
		Render(&thegame);
	}

	//waiting for players loop (practically nothing displayed on screen)
	thegame.messages.AddMessage("connected, waiting for other players", 5.0f);
	while(true)
	{
		seconds = updateTime();
		Input(&InputData);
		thegame.messages.Run(seconds);
		if(Client.messages.size())
		{
			if((*Client.messages.begin()) == "_menu")
			{
				Client.messages.pop_front();
				break;
			}
			else
			{
				thegame.messages.AddMessage((*Client.messages.begin()), 5.0f);
				Client.messages.pop_front();
			}
		}
		Render(&thegame);
	}

	//menu loop
	thegame.messages.AddMessage("Pick your color", 5.0f);
	SetupMenu(&thegame);			//sets up a list of colored tiles for the user to pick their color
	while(!thegame.setup && !thegame.over && HandleMessages())
    {
		seconds = updateTime();
        Input(&InputData);
        RunMenuFrame(&thegame, &InputData, &Client, seconds);
        Render(&thegame);
    }

	//main game loop
    while(HandleMessages() && !thegame.over)
    {
		seconds = updateTime();
        Input(&InputData);
        RunFrame(&thegame, &InputData, &Client, seconds);
        Render(&thegame);
    }
	thegame.messages.AddMessage("Waiting for goodbye from server.", 5.0f);
	Render(&thegame);
	thegame.logFile.close();
}

float updateTime()
{
	static DWORD startingPoint = GetTickCount();
	static DWORD deltaTime;
	deltaTime = GetTickCount() - startingPoint;
	startingPoint = GetTickCount();
	return deltaTime / 1000.0f;
}