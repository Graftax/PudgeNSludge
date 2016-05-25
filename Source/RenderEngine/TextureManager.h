//////////////////////////////////////////////////////////////////////////////////////
// Filename:            TextureManager.h
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all textures
//////////////////////////////////////////////////////////////////////////////////////
#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include "RenderEngine.h"
#include "../PS/CGame.h"
#include <string>
#include <unordered_map>

//structure storing all information about a texture
__declspec(align(16))
struct textureData
{
	std::string m_fileName;//actual name of the texture
	char m_filePath[MAX_PATH];//the path to the texture
	unsigned int m_textureHandle;//the index in the map
	unsigned int m_uiRefCount;//how many people are using us
	unsigned int m_uiWidth;//width of the texture
	unsigned int m_uiHeight;//height of the texture
	LPDIRECT3DTEXTURE9 m_lpTexture;//the actual texture

	textureData(void)//default me!
	{
		m_fileName = "";
		m_filePath[0] = '\0';
		m_textureHandle = 0u;
		m_uiRefCount = 0u;
		m_uiWidth = 0u;
		m_uiHeight = 0u;
		m_lpTexture = nullptr;
	}
};

//singleton class in control of loading and unloading textures
__declspec(align(32))
class TextureManager
{
private:
	//friends need us to be there for them! :D
	friend class RenderEngine;
	friend class CComponent_Input;
	friend class CGame;
	//The one and only!
	static TextureManager* m_pInstance;

	INT m_defaultHandle;

	TextureManager(){}									// Private so that it can  not be called
	TextureManager(TextureManager const&){}				// copy constructor is private
	TextureManager& operator=(TextureManager const&){}  // assignment operator is private

	//Map of all textureData that has been loaded 
	std::unordered_map<unsigned int, textureData> m_umTextureMap;
	//Next available index into the map
	UINT m_uiNextIndex;

public:	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// InitTextureManager(): Start up the texture manager.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void InitTextureManager(void);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// LoadTexture(): Load the texture and add it to the map.
	//                       
	// Returns: int - index into the map where the texture is at.
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int LoadTexture( const char* _szFileName );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// ShutdownTextureManager(): Unload all texture that have been loaded.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ShutdownTextureManager(void);	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// UnloadTexture(): Unload the texture at the specific index.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UnloadTexture( unsigned int _uiIndex );
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// UnloadTexture(): Unload the passed in texture.
	//                       
	// Returns: void
	//                            
	//
	// Mod. Name: Bryan Sauter
	// Mod. Date: 5/24/12
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void UnloadTexture( LPDIRECT3DTEXTURE9 _texture );

	/////////////////////////////
	//  Accessors
	/////////////////////////////
	LPDIRECT3DTEXTURE9 GetTexture( unsigned int _uiIndex );
	static TextureManager* GetInstance(void);

	void UnloadAllTextures(void);
	void ReloadAllTextures(void);
};

#endif //TEXTUREMANAGER_H_