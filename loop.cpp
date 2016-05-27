#include "inputdata.h"
#include "game.h"
#include "NetworkClient.h"

void LoadGraphics(Game* thegame);
bool HandleMessages();
void MainMenu(Game* thegame);
void Input(INPUTDATA* InputData);
void Render(Game* thegame);
void MenuLogic(Game* thegame, INPUTDATA* InputData, NetworkClient* Client);
void Logic(Game* thegame, INPUTDATA* InputData, NetworkClient* Client);

// The Main Loop
void MainLoop(const char* ServerAddress)
{
	Game thegame;
	INPUTDATA InputData;
	NetworkClient Client(ServerAddress);
	LoadGraphics(&thegame);
	MainMenu(&thegame);			//displays a list of colored tiles for the use to pick their color
	while(!thegame.setup && !thegame.over && HandleMessages())
    {
        Input(&InputData);
        MenuLogic(&thegame, &InputData, &Client);
        Render(&thegame);
    }

    while(HandleMessages() && !thegame.over)
    {
        Input(&InputData);
        Logic(&thegame, &InputData, &Client);
        Render(&thegame);
    }
}