#include <d3d9.h>
#include <d3dx9.h>
using namespace std;

class MODEL
{
public:
    LPD3DXMESH Mesh;
    D3DMATERIAL9* Material;
    LPDIRECT3DTEXTURE9* Texture;
    DWORD numMaterials;
	int modelNum;

    ~MODEL()
    {
        delete [] Material;
        delete [] Texture;
    }
};