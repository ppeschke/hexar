#include <atlbase.h>
#include <stdlib.h>
#include<string>
using namespace std;

#include "gamewindow.h"
#include "model.h"
#include "color.h"

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
D3DPRESENT_PARAMETERS d3dpp;
LPD3DXSPRITE d3dspt;
LPD3DXFONT dxfont;
LPD3DXFONT dxfontpeschkes;
LPD3DXSPRITE ddraw;
LPDIRECT3DTEXTURE9 chatWindow;

//direct3d.cpp prototypes
void InitDirect3D(GAMEWINDOW* gw);
void CloseDirect3D();
void StartRender();
void EndRender();
void LoadModel(MODEL* Model, LPCTSTR File, int modelNum);
void SetCamera();
void AdjustCamera(float x, float y, float z);
void DrawModel(MODEL* Model, float x, float y, float z, float scale, float rotation, color c);
void InitLight();
void drawChat(string msg);
void DrawSprite(LPDIRECT3DTEXTURE9 s, int x, int y);
void StartSpriteRender();
void EndSpriteRender();

// include the Direct3D Library files so you don't have to put them in the project settings
//(the paths to these do have to be included in the lib directories for the compiler though)
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

// Create Direct3D and the Direct3D Device
void InitDirect3D(GAMEWINDOW* gw)
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.Windowed = gw->Windowed;
    d3dpp.BackBufferWidth = gw->Width;
    d3dpp.BackBufferHeight = gw->Height;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                                D3DDEVTYPE_HAL,
                                gw->hWnd,
                                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                &d3dpp,
                                &d3ddev);
	D3DXCreateFont(d3ddev, 20, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
               DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, (LPCSTR)"Arial", &dxfont);
	D3DXCreateFont(d3ddev, 28, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH || FF_DONTCARE, (LPCSTR)"Arial", &dxfontpeschkes);

	D3DXCreateSprite(d3ddev, &ddraw);

    d3ddev->SetRenderState(D3DRS_LIGHTING, TRUE);
    d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
	d3ddev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);    // handle normals in scaling
	D3DXCreateTextureFromFile(d3ddev, (LPCSTR)"chatwindow.png", &chatWindow);
    return;
}

// Close the Device and Direct3D
void CloseDirect3D()
{
	dxfont->Release();
	dxfontpeschkes->Release();
    d3ddev->Release();
    d3d->Release();

    return;
}

// Start rendering
void StartRender()
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->BeginScene();

    return;
}

// Stop rendering
void EndRender()
{
    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);

    return;
}

// Load a mesh
void LoadModel(MODEL* Model, LPCTSTR File, int num)
{
    LPD3DXBUFFER bufMaterial;

    D3DXLoadMeshFromX(File,
                      D3DXMESH_SYSTEMMEM,
                      d3ddev,
                      NULL,
                      &bufMaterial,
                      NULL,
                      &Model->numMaterials,
                      &Model->Mesh);

    D3DXMATERIAL* tempMaterials = (D3DXMATERIAL*)bufMaterial->GetBufferPointer();

    Model->Material = new D3DMATERIAL9[Model->numMaterials];
    Model->Texture = new LPDIRECT3DTEXTURE9[Model->numMaterials];
	Model->modelNum = num;

    for(DWORD index = 0; index < Model->numMaterials; index++)
    {
        Model->Material[index] = tempMaterials[index].MatD3D;
        Model->Material[index].Ambient = Model->Material[index].Diffuse;

        USES_CONVERSION;
        if(FAILED(D3DXCreateTextureFromFile(d3ddev,
                                            tempMaterials[index].pTextureFilename,
                                            &Model->Texture[index])))
            Model->Texture[index] = NULL;
    }

    return;
}

// Set the fixed properties of the 3D camera
void SetCamera()
{
    D3DXMATRIX matProjection;
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               D3DXToRadian(45),
                               (FLOAT)d3dpp.BackBufferWidth/(FLOAT)d3dpp.BackBufferHeight,
                               1.0f,
                               100.0f);

    d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);

    return;
}

// Set the adjustable properties of the 3D camera
void AdjustCamera(float x, float y, float z)
{
    D3DXMATRIX matView;

    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3 (x, y, z),
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));

    d3ddev->SetTransform(D3DTS_VIEW, &matView);

    return;
}

// Draw the Specified Mesh
void DrawModel(MODEL* Model, float x, float y, float z, float scale, float rotation, color c)
{
    D3DXMATRIX matTranslate;
	D3DXMATRIX matRotateY;
	D3DXMATRIX matScale;
	D3DXMatrixTranslation(&matTranslate, x, y, z);
	D3DXMatrixRotationY(&matRotateY, rotation);
	D3DXMatrixScaling(&matScale, scale, scale, scale);
    d3ddev->SetTransform(D3DTS_WORLD, &(matScale * matRotateY * matTranslate));
	D3DMATERIAL9* tempMaterial = Model->Material;
	switch(c)
	{
	case red:
		tempMaterial->Diffuse.r = 1.0f;
		tempMaterial->Diffuse.g = 0.0f;
		tempMaterial->Diffuse.b = 0.0f;
		break;
	case green:
		tempMaterial->Diffuse.r = 0.0f;
		tempMaterial->Diffuse.g = 1.0f;
		tempMaterial->Diffuse.b = 0.0f;
		break;
	case blue:
		tempMaterial->Diffuse.r = 0.0f;
		tempMaterial->Diffuse.g = 0.0f;
		tempMaterial->Diffuse.b = 1.0f;
		break;
	case yellow:
		tempMaterial->Diffuse.r = 1.0f;
		tempMaterial->Diffuse.g = 1.0f;
		tempMaterial->Diffuse.b = 0.0f;
		break;
	case orange:
		tempMaterial->Diffuse.r = 1.0f;
		tempMaterial->Diffuse.g = 0.5f;
		tempMaterial->Diffuse.b = 0.0f;
		break;
	case cyan:
		tempMaterial->Diffuse.r = 0.0f;
		tempMaterial->Diffuse.g = 1.0f;
		tempMaterial->Diffuse.b = 1.0f;
		break;
	default:	//white
		tempMaterial->Diffuse.r = 1.0f;
		tempMaterial->Diffuse.g = 1.0f;
		tempMaterial->Diffuse.b = 1.0f;
		break;
	}
	/*tempMaterial->Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	tempMaterial->Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);*/

    for(DWORD index = 0; index < Model->numMaterials; index++)
    {
        d3ddev->SetMaterial(tempMaterial);
        if(Model->Texture[index] != NULL)
            d3ddev->SetTexture(0, Model->Texture[index]);

        Model->Mesh->DrawSubset(index);
    }

    return;
}

void InitLight()
{
    D3DMATERIAL9 material;    // create the material struct

	D3DLIGHT9 light;    // create the light struct
	ZeroMemory(&light, sizeof(light));    // clear out the light struct for use
    light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
    light.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);    // set the light's color
    light.Position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	light.Direction = D3DXVECTOR3(0.5f, -1.0f, -1.0f);
    d3ddev->SetLight(0, &light);    // send the light struct properties to light #0
    d3ddev->LightEnable(0, TRUE);    // turn on light #0

	D3DLIGHT9 pointLight;    // create the light struct
	ZeroMemory(&pointLight, sizeof(pointLight));    // clear out the light struct for use
	pointLight.Type = D3DLIGHT_POINT;    // make the light type 'point light'
	pointLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set the light's color
	pointLight.Position = D3DXVECTOR3(0.0f, 7.0f, 5.0f);
	//pointLight.Direction = D3DXVECTOR3(0.5f, -1.0f, -1.0f);
	pointLight.Range = 100.0f;
	pointLight.Attenuation0 = 0.0f;    // no constant inverse attenuation
	pointLight.Attenuation1 = 0.125f;    // only .125 inverse attenuation
	pointLight.Attenuation2 = 0.0f;    // no square inverse attenuation
	d3ddev->SetLight(1, &pointLight);    // send the light struct properties to light #0
	d3ddev->LightEnable(1, TRUE);    // turn on light #1

    ZeroMemory(&material, sizeof(D3DMATERIAL9));    // clear out the struct for use
    material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set diffuse color to white
    material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // set ambient color to white

    d3ddev->SetMaterial(&material);    // set the globably-used material to &material
}

void drawTextAt(const char* text, RECT* textbox)
{
	dxfont->DrawTextA(NULL, text, strlen(text), textbox, DT_LEFT | DT_TOP | DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 0));
}

void drawChat(string msg)
{
	static RECT textbox{ 10, 586, 341, 1024 };
	StartSpriteRender();
	DrawSprite(chatWindow, 0, 576);
	EndSpriteRender();
	dxfont->DrawTextA(NULL, msg.c_str(), msg.size(), &textbox, DT_LEFT | DT_TOP | DT_WORDBREAK, D3DCOLOR_ARGB(255, 255, 255, 0));
}

void DrawSprite(LPDIRECT3DTEXTURE9 s, int x, int y)
{
	HRESULT hr = ddraw->Draw(s, NULL, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3((float)x, (float)y, 0.0f), D3DCOLOR_XRGB(255, 255, 255));
	//s->sprite	is the sprite to draw
	//NULL tells it to draw the whole stinkin' thing (would normally be a pRECT
	//D3DXVECTOR3 is the center point (just setting it to (0, 0)
	//the second D3DVECTOR3 is the point to start drawing it on the screen
	//the color is to draw at full intensity
	/*HRESULT Draw(LPDIRECT3DTEXTURE9 pTexture,
			 CONST RECT* pSrcRect,
			 CONST D3DXVECTOR3* pCenter,
			 CONST D3DXVECTOR3* pPosition,
			 D3DCOLOR Color);*/
}

void drawPeschkes(const char* peschkes)
{
	static RECT textbox;
	SetRect(&textbox, 224, 700, 1014, 758);
	dxfontpeschkes->DrawTextA(NULL, peschkes, strlen(peschkes), &textbox, DT_RIGHT | DT_BOTTOM,
					  D3DCOLOR_ARGB(255, 255, 255, 255));
}

void StartSpriteRender()
{
	//ddraw->Begin(NULL);
	ddraw->Begin(D3DXSPRITE_ALPHABLEND);		// begin sprite drawing with transparency
}

void EndSpriteRender()
{
	ddraw->End();
}