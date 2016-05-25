//////////////////////////////////////////////////////////////////////////////////////
// Filename: CCamera.cpp
// Author: Kevin Clouden
// Date: 5/8/12
// Purpose: The camera class holds all viewing information for DirectX. 
// It will be used by the Input Manager and the Renderer. Camera will get the players
// position from the object manager
//////////////////////////////////////////////////////////////////////////////////////
#include "Camera.h"
#include "PS RenderEngine\RenderEngine.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors: Default constructor / Destructor  
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCamera::CCamera()
{
	D3DXMatrixIdentity(&this->m_ProjectionMatrix);
	D3DXMatrixIdentity(&this->m_OrthographicMatrix);
	D3DXMatrixIdentity(&this->m_ViewMatrix);

	endpoint = vec3f(0.0f, 0.0f, 0.0f);
}

CCamera::~CCamera()
{

}

bool CCamera::Initialize(IDirect3DDevice9* device)
{
	l_pd3dDevice = device;
	return true;
}

void CCamera::Shutdown(void)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accesssors: Returns data 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
D3DXMATRIX CCamera::GetProjectionMatrix()
{
	return this->m_ProjectionMatrix;
}

D3DXMATRIX CCamera::GetOrthograhicMatrix()
{
	return this->m_OrthographicMatrix;
}

D3DXMATRIX CCamera::GetViewMatrix(bool _bTranslate)
{
	D3DXMATRIX _mRotMatrix(this->m_ViewMatrix), _mTransform;
	_mRotMatrix._41 = _mRotMatrix._42 = _mRotMatrix._43 = 0;
	D3DXMatrixTranspose(&_mRotMatrix, &_mRotMatrix);

	D3DXMatrixIdentity(&_mTransform);
	_mTransform._41 = -(this->m_ViewMatrix._41);
	_mTransform._42 = -(this->m_ViewMatrix._42);
	_mTransform._43 = -(this->m_ViewMatrix._43);

	D3DXMatrixMultiply(&_mTransform, &_mTransform, &_mRotMatrix);
	return (_bTranslate ? _mTransform : _mRotMatrix);
}

D3DXVECTOR3 CCamera::GetViewXAxis()
{
	return D3DXVECTOR3(m_ViewMatrix._11, m_ViewMatrix._12, m_ViewMatrix._13);
}

D3DXVECTOR3 CCamera::GetViewYAxis()
{
	return D3DXVECTOR3(m_ViewMatrix._21, m_ViewMatrix._22, m_ViewMatrix._23);
}

D3DXVECTOR3 CCamera::GetViewZAxis()
{
	return D3DXVECTOR3(m_ViewMatrix._31, m_ViewMatrix._32, m_ViewMatrix._33);
}

D3DXVECTOR3 CCamera::GetViewPosition()
{
	return D3DXVECTOR3(m_ViewMatrix._41, m_ViewMatrix._42, m_ViewMatrix._43);
}
vec3f CCamera::GetWorldCursor()
{
	return m_Cursor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Mutators: Sets data 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::SetProjectionMatrix(D3DXMATRIX* matrix)
{
	memcpy(&this->m_ProjectionMatrix, matrix, sizeof(D3DXMATRIX));
}

void CCamera::SetViewMatrix(D3DXMATRIX* matrix)
{
	memcpy(&this->m_ViewMatrix, matrix, sizeof(D3DXMATRIX));
}

void CCamera::SetViewXAxis(D3DXVECTOR3 _xPos)
{
	this->m_ViewMatrix._11 = _xPos.x;
	this->m_ViewMatrix._12 = _xPos.y;
	this->m_ViewMatrix._13 = _xPos.z;
}

void CCamera::SetViewXAxis(float _x, float _y, float _z)
{
	this->m_ViewMatrix._11 = _x;
	this->m_ViewMatrix._12 = _y;
	this->m_ViewMatrix._13 = _z;
}

void CCamera::SetViewYAxis(D3DXVECTOR3 _yPos)
{
	this->m_ViewMatrix._21 = _yPos.x;
	this->m_ViewMatrix._22 = _yPos.y;
	this->m_ViewMatrix._23 = _yPos.z;
}

void CCamera::SetViewYAxis(float _x, float _y, float _z)
{
	this->m_ViewMatrix._21 = _x;
	this->m_ViewMatrix._22 = _y;
	this->m_ViewMatrix._23 = _z;
}

void CCamera::SetViewZAxis(D3DXVECTOR3 _zPos)
{
	this->m_ViewMatrix._31 = _zPos.x;
	this->m_ViewMatrix._32 = _zPos.y;
	this->m_ViewMatrix._33 = _zPos.z;
}

void CCamera::SetViewZAxis(float _x, float _y, float _z)
{
	this->m_ViewMatrix._31 = _x;
	this->m_ViewMatrix._32 = _y;
	this->m_ViewMatrix._33 = _z;
}

void CCamera::SetViewPosition(D3DXVECTOR3 _Pos)
{
	this->m_ViewMatrix._41 = _Pos.x;
	this->m_ViewMatrix._42 = _Pos.y;
	this->m_ViewMatrix._43 = _Pos.z;
}

void CCamera::SetViewPosition(float _x, float _y, float _z)
{
	this->m_ViewMatrix._41 = _x;
	this->m_ViewMatrix._42 = _y;
	this->m_ViewMatrix._43 = _z;
}
void CCamera::SetWorldCursor(vec3f cursor)
{
	m_Cursor = cursor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BuildPerspective: Builds the projectionMatrix 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::BuildPerspective(float _fov, float _aspect, float _near, float _far)
{
	D3DXMatrixPerspectiveFovLH(&this->m_ProjectionMatrix, _fov, _aspect, _near, _far);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BuildOrthographic: Builds the orthographic matrix 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::BuildOrthographic(float _width, float _height, float _near, float _far)
{
	D3DXMatrixOrthoLH(&this->m_OrthographicMatrix, _width, _height, _near, _far);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RotateLocalX: Rotates the camera on the local X 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateLocalX(float _angle)
{
	D3DXMATRIX  _mRotation;
	D3DXVECTOR3 _vLocalX(m_ViewMatrix._11, m_ViewMatrix._12, m_ViewMatrix._13);
	D3DXMatrixRotationAxis(&_mRotation, &_vLocalX, _angle);

	D3DXVECTOR4 Position(this->m_ViewMatrix._41, this->m_ViewMatrix._42, 
		this->m_ViewMatrix._43, this->m_ViewMatrix._44);
	this->m_ViewMatrix._41 = this->m_ViewMatrix._42 = this->m_ViewMatrix._43 = 0.0f; 

	D3DXMatrixMultiply(&m_ViewMatrix, &m_ViewMatrix, &_mRotation);
	this->m_ViewMatrix._41 = Position.x; this->m_ViewMatrix._42 = Position.y;
	this->m_ViewMatrix._43 = Position.z; this->m_ViewMatrix._44 = Position.w;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RotateLocalY: Rotates the camera on the local Y 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateLocalY(float _angle)
{
	D3DXMATRIX  _mRotation;
	D3DXVECTOR3 _vLocalY(m_ViewMatrix._21, m_ViewMatrix._22, m_ViewMatrix._23);
	D3DXMatrixRotationAxis(&_mRotation, &_vLocalY, _angle);

	D3DXVECTOR4 Position(this->m_ViewMatrix._41, this->m_ViewMatrix._42, 
		this->m_ViewMatrix._43, this->m_ViewMatrix._44);
	this->m_ViewMatrix._41 = this->m_ViewMatrix._42 = this->m_ViewMatrix._43 = 0.0f; 

	D3DXMatrixMultiply(&m_ViewMatrix, &m_ViewMatrix, &_mRotation);
	this->m_ViewMatrix._41 = Position.x; this->m_ViewMatrix._42 = Position.y;
	this->m_ViewMatrix._43 = Position.z; this->m_ViewMatrix._44 = Position.w;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RotateLocalZ: Rotates the camera on the local Z 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateLocalZ(float _angle)
{
	D3DXMATRIX  _mRotation;
	D3DXVECTOR3 _vLocalZ(m_ViewMatrix._31, m_ViewMatrix._32, m_ViewMatrix._33);
	D3DXMatrixRotationAxis(&_mRotation, &_vLocalZ, _angle);

	D3DXVECTOR4 Position(this->m_ViewMatrix._41, this->m_ViewMatrix._42, 
		this->m_ViewMatrix._43, this->m_ViewMatrix._44);
	this->m_ViewMatrix._41 = this->m_ViewMatrix._42 = this->m_ViewMatrix._43 = 0.0f; 

	D3DXMatrixMultiply(&m_ViewMatrix, &m_ViewMatrix, &_mRotation);
	this->m_ViewMatrix._41 = Position.x; this->m_ViewMatrix._42 = Position.y;
	this->m_ViewMatrix._43 = Position.z; this->m_ViewMatrix._44 = Position.w;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateLocal: Translates the camera on the local axis
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateLocal(D3DXVECTOR3 _vAxis)
{
	TranslateLocalX(_vAxis.x);
	TranslateLocalY(_vAxis.y);
	TranslateLocalZ(_vAxis.z);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateLocalX: Translates the camera on the local X 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateLocalX(float _scale)
{
	this->m_ViewMatrix._41 += (this->m_ViewMatrix._11 * _scale);
	this->m_ViewMatrix._42 += (this->m_ViewMatrix._12 * _scale);
	this->m_ViewMatrix._43 += (this->m_ViewMatrix._13 * _scale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateLocalY: Translates the camera on the local Y 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateLocalY(float _scale)
{
	this->m_ViewMatrix._41 += (this->m_ViewMatrix._21 * _scale);
	this->m_ViewMatrix._42 += (this->m_ViewMatrix._22 * _scale);
	this->m_ViewMatrix._43 += (this->m_ViewMatrix._23 * _scale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateLocalZ: Translates the camera on the local Z 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateLocalZ(float _scale)
{
	this->m_ViewMatrix._41 += (this->m_ViewMatrix._31 * _scale);
	this->m_ViewMatrix._42 += (this->m_ViewMatrix._32 * _scale);
	this->m_ViewMatrix._43 += (this->m_ViewMatrix._33 * _scale);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RotateGlobalX: Rotates the camera on the global X 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateGlobalX(float _angle)
{
	D3DXMATRIX _mRotation;
	D3DXVECTOR4 Position(this->m_ViewMatrix._41, this->m_ViewMatrix._42, 
		this->m_ViewMatrix._43, this->m_ViewMatrix._44);
	D3DXMatrixRotationX(&_mRotation, _angle);

	this->m_ViewMatrix._41 = this->m_ViewMatrix._42 = this->m_ViewMatrix._43 = 0.0f; 

	D3DXMatrixMultiply(&this->m_ViewMatrix, &this->m_ViewMatrix, &_mRotation);
	this->m_ViewMatrix._41 = Position.x; this->m_ViewMatrix._42 = Position.y;
	this->m_ViewMatrix._43 = Position.z; this->m_ViewMatrix._44 = Position.w;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RotateGlobalY: Rotates the camera on the global Y 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateGlobalY(float _angle)
{
	D3DXMATRIX _mRotation;
	D3DXVECTOR4 Position(this->m_ViewMatrix._41, this->m_ViewMatrix._42, 
		this->m_ViewMatrix._43, this->m_ViewMatrix._44);
	D3DXMatrixRotationY(&_mRotation, _angle);

	this->m_ViewMatrix._41 = this->m_ViewMatrix._42 = this->m_ViewMatrix._43 = 0.0f; 

	D3DXMatrixMultiply(&this->m_ViewMatrix, &this->m_ViewMatrix, &_mRotation);
	this->m_ViewMatrix._41 = Position.x; this->m_ViewMatrix._42 = Position.y;
	this->m_ViewMatrix._43 = Position.z; this->m_ViewMatrix._44 = Position.w;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RotateGlobalZ: Rotates the camera on the global Z 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::RotateGlobalZ(float _angle)
{
	D3DXMATRIX _mRotation;
	D3DXVECTOR4 Position(this->m_ViewMatrix._41, this->m_ViewMatrix._42, 
		this->m_ViewMatrix._43, this->m_ViewMatrix._44);
	D3DXMatrixRotationZ(&_mRotation, _angle);

	this->m_ViewMatrix._41 = this->m_ViewMatrix._42 = this->m_ViewMatrix._43 = 0.0f; 

	D3DXMatrixMultiply(&this->m_ViewMatrix, &this->m_ViewMatrix, &_mRotation);
	this->m_ViewMatrix._41 = Position.x; this->m_ViewMatrix._42 = Position.y;
	this->m_ViewMatrix._43 = Position.z; this->m_ViewMatrix._44 = Position.w;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateGlobal: translate the camera on the global axis 
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateGlobal(D3DXVECTOR3 _vAxis)
{
	this->m_ViewMatrix._41 += _vAxis.x;
	this->m_ViewMatrix._42 += _vAxis.y;
	this->m_ViewMatrix._43 += _vAxis.z;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateGlobalX: translate the camera on the global X axis
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateGlobalX(float _scale)
{
	this->m_ViewMatrix._41 += _scale;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateGlobalY: translate the camera on the global Y axis
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateGlobalY(float _scale)
{
	this->m_ViewMatrix._42 += _scale;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TranslateGlobalZ: translate the camera on the global Z axis
// Returns: void
// Mod. Name: KC
// Mod. Date: 5/8/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCamera::TranslateGlobalZ(float _scale)
{
	this->m_ViewMatrix._43 += _scale;
}

void CCamera::MoveCamWithMouse( vec3f playerPos, HWND wnd)
{
	//vec3f tCursorPos = *((vec3f*)(&m_pInput->GetCursorPosition()));
	////Sets the camera's y so its not inverted
	//float tempos = (float)tCursorPos.y;
	//tempos = 768.0f - tempos;

	////Sets the players pos var
	//D3DXVECTOR3 xPlayerPos;
	//xPlayerPos.x = playerPos.x;
	//xPlayerPos.y = playerPos.y;
	//xPlayerPos.z = playerPos.z;

	////Calculation for camera. Camera is moved by mouse cuz of this equation
	//D3DXVECTOR3 camPos = ( ( xPlayerPos + ( D3DXVECTOR3( tCursorPos.x, tempos, 0.0f ) / 2.0f )) );

	////Cursor in world space calculations 
	//m_Cursor = vec3f( (float)(tCursorPos.x), tempos, 0.0f ) * 2.0f + vec3f( xPlayerPos.x, xPlayerPos.y, xPlayerPos.z );

	////Camera offset. Aka zoom in or out
	//camPos += D3DXVECTOR3( 0.0f, 0.0f, -1000.0f );

	////Sets the camera pos
	//SetViewPosition(camPos);
}
