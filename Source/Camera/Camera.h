//////////////////////////////////////////////////////////////////////////////////////
// Filename: CCamera.h
// Author: Kevin Clouden
// Date: 5/8/12
// Purpose: The camera class holds all viewing information for DirectX. 
// It will be used by the Input Manager and the Renderer. Camera will get the players
// position from the object manager
//////////////////////////////////////////////////////////////////////////////////////

#ifndef CCAMERA_H_
#define CCAMERA_H_

#include "../PS/config.h"
#include "..//Utilities/MathLib/matrix4.h"

class CGame;
class CInputManager;


class CCamera
{
private:
	D3DXMATRIX m_ProjectionMatrix;
	D3DXMATRIX m_OrthographicMatrix;
	D3DXMATRIX m_ViewMatrix;
	CGame* m_pGame;
	CInputManager* m_pInput;
	vec3f m_Cursor;
	vec3f endpoint;

	// These Variables will be Used for Interpolating over time.
	vec3f CurrentCameraPositionForInterpolation;
	vec3f FutureCameraPositionForInterpolation;
	float InterpolateLambda;
public:
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Constructors: Default constructor / Destructor  
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CCamera();
	~CCamera();

	bool Initialize(void);
	void Shutdown(void);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Accessors: Returns data 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3DXMATRIX GetProjectionMatrix();
	D3DXMATRIX GetOrthograhicMatrix();
	D3DXMATRIX GetViewMatrix(bool _bTranslate = true);
	D3DXMATRIX GetWorldMatrix() { return m_ViewMatrix;}
	D3DXVECTOR3 GetViewXAxis();
	D3DXVECTOR3 GetViewYAxis();
	D3DXVECTOR3 GetViewZAxis();
	D3DXVECTOR3 GetViewPosition();
	vec3f GetWorldCursor();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Mutators: Sets data 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetProjectionMatrix(D3DXMATRIX* matrix);
	void SetViewMatrix(D3DXMATRIX* matrix);
	void SetViewXAxis(D3DXVECTOR3 _xPos);
	void SetViewXAxis(float _x, float _y, float _z);
	void SetViewYAxis(D3DXVECTOR3 _yPos);
	void SetViewYAxis(float _x, float _y, float _z);
	void SetViewZAxis(D3DXVECTOR3 _zPos);
	void SetViewZAxis(float _x, float _y, float _z);
	void SetViewPosition(D3DXVECTOR3 _Pos);
	void SetViewPosition(float _x, float _y, float _z);
	void SetWorldCursor(vec3f cursor);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BuildPerspective: Builds the projectionMatrix 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildPerspective(float _fov, float _aspect, float _near, float _far);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BuildOrthographic: Builds the orthographic matrix 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void BuildOrthographic(float _width, float _height, float _near, float _far);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RotateLocalX: Rotates the camera on the local X 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RotateLocalX(float _angle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RotateLocalY: Rotates the camera on the local Y 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RotateLocalY(float _angle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RotateLocalZ: Rotates the camera on the local Z 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RotateLocalZ(float _angle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateLocal: Translates the camera on the local axis
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateLocal(D3DXVECTOR3 _vAxis);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateLocalX: Translates the camera on the local X 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateLocalX(float _scale);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateLocalY: Translates the camera on the local Y 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateLocalY(float _scale);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateLocalZ: Translates the camera on the local Z 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateLocalZ(float _scale);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RotateGlobalX: Rotates the camera on the global X 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RotateGlobalX(float _angle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RotateGlobalY: Rotates the camera on the global Y 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RotateGlobalY(float _angle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RotateGlobalZ: Rotates the camera on the global Z 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void RotateGlobalZ(float _angle);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateGlobal: translate the camera on the global axis 
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateGlobal(D3DXVECTOR3 _vAxis);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateGlobalX: translate the camera on the global X axis
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateGlobalX(float _scale);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateGlobalY: translate the camera on the global Y axis
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateGlobalY(float _scale);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// TranslateGlobalZ: translate the camera on the global Z axis
	// Returns: void
	// Mod. Name: KC
	// Mod. Date: 5/8/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void TranslateGlobalZ(float _scale);

	void MoveCamWithMouse(vec3f playerPos, HWND wnd);
	void SetLazerEndPoint(vec3f x){endpoint = x;}
	vec3f GetLazerEndPoint() {return endpoint;}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InterpolateCameraOverTime: Interpolates the Camera from one position to the next.
	// Returns: bool - whether its done interpolating or not.
	// Mod. Name: Andy Madruga
	// Mod. Date: 7/21/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool InterpolateCameraOverTime( float _fDT, vec3f PosTo );

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SetInterpolateCameraPosition: Sets the Current Camera Interpolate Position.
	// Returns: void
	// Mod. Name: Andy Madruga
	// Mod. Date: 7/21/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetInterpolateCameraPosition( void ) { CurrentCameraPositionForInterpolation = vec3f( GetViewPosition().x, GetViewPosition().y, GetViewPosition().z ); }
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SetInterpolateFuturePosition: Sets the Future Camera Interpolate Position.
	// Returns: void
	// Mod. Name: Andy Madruga
	// Mod. Date: 7/21/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetInterpolateFuturePosition( vec3f PosTo );
};

#endif // CCAMERA_H_