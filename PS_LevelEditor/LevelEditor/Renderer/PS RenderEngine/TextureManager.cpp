//////////////////////////////////////////////////////////////////////////////////////
// Filename:            TextureManager.cpp
// Author:              Bryan Sauter
// Date:                5/5/12
// Purpose:             A manager used for keeping track of all textures
//////////////////////////////////////////////////////////////////////////////////////
#include "TextureManager.h"


TextureManager* TextureManager::m_pInstance = nullptr;

TextureManager* TextureManager::GetInstance(void)
{
	//we are a singleton, so have we been made yet?
	if(!m_pInstance)
	{
		//if not create a new instance of us
		m_pInstance = new TextureManager;
		//init the manager
		//m_pInstance->InitTextureManager();
	}
	//return the current instance of the class
	return m_pInstance;
}
// LoadTexture(): Load the texture and add it to the map.
//                       
// Returns: int - index into the map where the texture is at.
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int TextureManager::LoadTexture( const char* _szFileName )
{
	//easy out, no file == nothing to load.
	if(!_szFileName) 
		return 0;
	//Easy out, if the texture is already loaded return its handle(index into the map)
	for( unsigned int i = 0u; i < m_umTextureMap.size(); ++i )
	{
		//check the filenames and if there is a texture actually loaded
		if( _stricmp( _szFileName, m_umTextureMap[i].m_filePath ) == 0 && m_umTextureMap[i].m_lpTexture != nullptr)
		{
			//add a reference to this texture
			m_umTextureMap[i].m_uiRefCount++;
			return i;
		}
	}
	//store the id for the next available texture slot
	int nId = m_uiNextIndex;
	//create a new texture data object in the map
	m_umTextureMap[nId] = textureData();
	//try to load the texture
	HRESULT result = 0;
	if( FAILED( result = D3DXCreateTextureFromFileA( RenderEngine::m_pD3Dev, (LPCSTR)(_szFileName), &m_umTextureMap[nId].m_lpTexture )))
	{
		//If we fail tell someone about it.
		char szBuffer[256] = {0};
		sprintf_s(szBuffer, "Failed to Create Texture - %s", _szFileName); 
		//MessageBoxA(0, LPCSTR(szBuffer), "TextureManager Error", MB_OK);
		//Actually tell everyone :)
		return m_defaultHandle;
	}
	else
	{
		//copy the file name into the new texturedata object
		strcpy_s( m_umTextureMap[nId].m_filePath, _szFileName );
		//go to the next available index as we will be filling the current one
		m_uiNextIndex++;
		//fill in more texture data
		m_umTextureMap[nId].m_textureHandle = nId;
		//add a referenece count
		m_umTextureMap[nId].m_uiRefCount++;
		//DRX object to get width and height of texture
		D3DSURFACE_DESC d3dSurfDesc;
		//zero the object memory just in case
		ZeroMemory( &d3dSurfDesc, sizeof(d3dSurfDesc) );
		//Get the description of the texture from DRX
		m_umTextureMap[nId].m_lpTexture->GetLevelDesc( 0, &d3dSurfDesc );
		//Fill in the height and width
		m_umTextureMap[nId].m_uiWidth = d3dSurfDesc.Width;
		m_umTextureMap[nId].m_uiHeight = d3dSurfDesc.Height;

		//REVERSE THE STRING FIRST
		int i = 0; std::string toAdd;//ADDER
		std::string reverse = _szFileName;//REVERSE STRING FOR READING
		reverse = std::string( reverse.rbegin(), reverse.rend());
		//REMOVE THE WHOLE FILE PATH EXCEPT FOR THE FILE NAME WHILE BACKWARDS
		while(true)
		{
			toAdd.push_back( reverse[i] );
			i++;
			if( reverse[i] == '\\' || reverse[i] == '/' )
				break;
		}
		//REMOVE FILE TYPE
		for( i = 0; i < 4; i++)
		{
			toAdd.erase( toAdd.begin());
		}
		//TURN IT BACK AROUND
		m_umTextureMap[nId].m_fileName = std::string( toAdd.rbegin(), toAdd.rend());
		//return the index into the map for this texture
		return nId;
	}	
	//we failed :'(
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InitTextureManager(): Start up the texture manager.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::InitTextureManager(void)
{
	//clear anything that may have been allocated in the map
	m_uiNextIndex = 0;
	m_umTextureMap.clear();
	m_defaultHandle = LoadTexture( "./Resources/Textures/TM_ERRORTEXTURE.png" );
	
	//LOG(LOG_INFO, "TextureManager::InitTextureManager(void)", "TextureManager was initialized!!");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ShutdownTextureManager(): Unload all texture that have been loaded.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::ShutdownTextureManager(void)
{
	//loop through the texture map
	for( unsigned int i = 0u; i < m_umTextureMap.size(); ++i )
	{
		//if there is a texture, free its memory
		if( m_umTextureMap[i].m_lpTexture )
			m_umTextureMap[i].m_lpTexture->Release();
	}
	//clear the size of the map
	m_umTextureMap.clear();
	//if we have created ourselves, delete ourselves.
	if( m_pInstance )
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

LPDIRECT3DTEXTURE9 TextureManager::GetTexture( unsigned int _uiIndex )
{
	//Get the texture from the map, no longer used. Very expensive
	if( m_umTextureMap[_uiIndex].m_lpTexture )
		return m_umTextureMap[_uiIndex].m_lpTexture;
	//if no texture return null
	return nullptr;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnloadTexture(): Unload the texture at the specific index.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::UnloadTexture( unsigned int _uiIndex )
{
	//check to make sure there is a texture at the specific index
	if( m_umTextureMap[_uiIndex].m_lpTexture )
	{
		std::unordered_map< unsigned int, textureData >::iterator iter( m_umTextureMap.begin() );
		for( unsigned int i = 0u; i < _uiIndex; ++i )
			iter++;
		//decrease the amount of references to this texture
		--m_umTextureMap[_uiIndex].m_uiRefCount;
		//if no one is using the texture unload it
		if( m_umTextureMap[_uiIndex].m_uiRefCount == 0 )
		{
			m_umTextureMap[_uiIndex].m_lpTexture->Release();
			m_umTextureMap[_uiIndex].m_lpTexture = nullptr;
			//m_umTextureMap.erase( iter );
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// UnloadTexture(): Unload the passed in texture.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void TextureManager::UnloadTexture( LPDIRECT3DTEXTURE9 _texture )
{
	std::unordered_map< unsigned int, textureData >::iterator iter( m_umTextureMap.begin() );
	//Loop through the texture map
	for( unsigned int i = 0u; i < m_umTextureMap.size(); ++i )
	{
		//if the texture is equal to the passed in texture
		if( m_umTextureMap[i].m_lpTexture == _texture )
		{
			//take away a reference to the texture
			--m_umTextureMap[i].m_uiRefCount;
			//check to make sure no one else is using the texture
			if( m_umTextureMap[i].m_uiRefCount == 0 )
			{
				//if its zero release the texture from memory
				m_umTextureMap[i].m_lpTexture->Release();
				m_umTextureMap[i].m_lpTexture = nullptr;
				//m_umTextureMap.erase( iter );
			}			
			//bye bye
			break;
		}
		++iter;
	}
}

void TextureManager::UnloadAllTextures(void)
{
	for( unsigned int i = 0u; i < m_umTextureMap.size(); ++i )
	{
		if( m_umTextureMap[i].m_lpTexture )
		{				
			m_umTextureMap[i].m_lpTexture->Release();
			m_umTextureMap[i].m_lpTexture = nullptr;
		}
	}
}

void TextureManager::ReloadAllTextures(void)
{
	for( unsigned int i = 0u; i < m_umTextureMap.size(); ++i )
	{
		D3DXCreateTextureFromFileA( RenderEngine::m_pD3Dev, (LPCSTR)(m_umTextureMap[i].m_filePath), &m_umTextureMap[i].m_lpTexture );
	}
}