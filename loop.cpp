#include "global.h"

// The Main Loop
void MainLoop(const char* ServerAddress)
{
	Game thegame;
	INPUTDATA InputData;
	client Client(ServerAddress, 17000);
	Client.listen();
	Client.connect();
	LoadGraphics(&thegame);
	MainMenu(&thegame);			//displays a list of colored tiles for the use to pick their color
	while(!thegame.setup && !thegame.over && HandleMessages())
    {
        Input(&InputData);
        MenuLogic(&thegame, &InputData, &Client);
        Render(&thegame, &Client);
    }

    while(HandleMessages() && !thegame.over)
    {
        Input(&InputData);
        Logic(&thegame, &InputData, &Client);
        Render(&thegame, &Client);
    }
}