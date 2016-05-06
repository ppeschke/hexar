// header files
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <atlbase.h>
#include <stdlib.h>
#include "gamewindow.h"
#include "object.h"
#include "inputdata.h"
#include "game.h"
#include "hexagon.h"
#include "rotator.h"
#include "client.h"
#include "action.h"

// libraries
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
//#pragma comment (lib, "dinput.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// prototypes for...
// WinMain.cpp
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void DisplayWindow(GAMEWINDOW* gw, HINSTANCE hInstance, int nCmdShow);
bool HandleMessages();

// Loop.cpp
void MainLoop(const char* ServerAddress);

// Direct3D.cpp
void InitDirect3D(GAMEWINDOW* gw);
void CloseDirect3D();
void StartRender();
void EndRender();
void LoadModel(MODEL* Model, LPCTSTR File, int modelNum);
void SetCamera();
void AdjustCamera(float x, float y, float z);
void DrawModel(MODEL* Model, float x, float y, float z, float scale, float rotation, color c);

// DirectInput.cpp
void InitDirectInput(HINSTANCE hInstance, GAMEWINDOW* gw);
void GetKeys(BYTE* KeyState);
void GetMouse(DIMOUSESTATE* MouseState);
void CloseDirectInput(void);
void InitLight();
void drawText(const char* text);

// Render.cpp
void Render(Game* thegame, client* Client);
void LoadGraphics(Game* thegame);

// Logic.cpp
void Logic(Game* thegame, INPUTDATA* InputData, client* Client);
void InitGame(Game* thegame, int playerNum, int rings);
void MainMenu(Game* thegame);
void MenuLogic(Game* thegame, INPUTDATA* InputData, client* Client);

// Input.cpp
void Input(INPUTDATA* InputData);