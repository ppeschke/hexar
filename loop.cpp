#include "inputdata.h"
#include "InputProcessor.h"
#include "NetworkClient.h"

void LoadGraphics(Game* thegame);
bool HandleMessages();
void SetupMenu(Game* thegame);
//void Input(INPUTDATA* InputData);
void HandleInputs(list<InputEvent>* ev, Game* thegame);
void Render(Game* thegame);
void RunMenuFrame(Game* thegame, NetworkClient* Client, float seconds);
void RunFrame(Game* thegame, NetworkClient* Client, float seconds);
inline float updateTime();

// The Outer Loop
void OuterLoop(const char* ServerAddress)
{
	Game thegame;
	thegame.logFile.open("log.txt");
//	INPUTDATA InputData;
	InputProcessor in(&thegame);
	list<InputEvent>* events = in.getEvents();
	NetworkClient Client(ServerAddress);
	LoadGraphics(&thegame);
	float seconds = updateTime();

	//connecting loop
	thegame.messages.AddMessage("connecting with " + string(ServerAddress), 5.0f);
	float connectionAttemptTimer = seconds;
	float prev = 0.0;
	while(!Client.Connected() && !thegame.over)
	{
		seconds = updateTime();
		connectionAttemptTimer += seconds;
		in.ProcessInputs();
		//Input(&InputData);
		HandleInputs(events, &thegame);
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
	while(!thegame.over)
	{
		seconds = updateTime();
		in.ProcessInputs();
		HandleInputs(events, &thegame);
		//Input(&InputData);
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
		in.ProcessInputs();
		HandleInputs(events, &thegame);
        //Input(&InputData);
        RunMenuFrame(&thegame, &Client, seconds);
        Render(&thegame);
    }

	//main game loop
    while(HandleMessages() && !thegame.over)
    {
		seconds = updateTime();
		in.ProcessInputs();
		HandleInputs(events, &thegame);
        //Input(&InputData);
        RunFrame(&thegame, &Client, seconds);
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

void HandleInputs(list<InputEvent>* ev, Game* thegame)
{
	while(!ev->empty())
	{
		InputEvent e = (*ev->begin());
		ev->pop_front();
		if(e.getType() == keyboard)
		{
			switch(e.getPressed())
			{
			case (char)27:
				thegame->over = true;
				break;
			default:
				thegame->over = true;
			}
		}
		else
		{
			if(e.getPressed() == '!')
			{
				if(thegame->arrow != nullptr)
				{
					thegame->arrow->x -= e.getX() / 20.0f;	//implement these later!
					thegame->arrow->z += e.getZ() / 20.0f;
				}
			}
			else
			{
				//click
			}
		}
	}
}