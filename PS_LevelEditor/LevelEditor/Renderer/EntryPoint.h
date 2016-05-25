#pragma once

#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include "Camera.h"
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )

#include "PS RenderEngine\RenderEngine.h"
#include "IBaseObject.h"

std::vector <D3DXVECTOR3>& GetHazards ();
std::vector<int>& GetHazardTypes ();

std::vector <IBaseObject*>& GetObjects ();
std::vector<int>& GetObjectTypes ();

CCamera* GetCam();


HRESULT InitD3D( HWND hWnd, int width, int height );
VOID SetupMatrices();
void Update (float fElapsedTime);
VOID Render();
RenderEngine* GetRenderer ();

void LoadLevel (char* _pFileName);
void SwapLevel (char* _pFileName);
void ReloadRenderer ();