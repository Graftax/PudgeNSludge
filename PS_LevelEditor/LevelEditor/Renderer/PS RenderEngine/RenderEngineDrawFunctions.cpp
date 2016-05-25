#include "RenderEngine.h"
#include "RenderSetSorted.h"
#include "RenderSet.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "Mesh.h"
#include "RenderTarget.h"
#include "../ObjectManager.h"

#include "LightSystem\Light.h"
#include "LightSystem\PointLight.h"
#include "LightSystem\DirectionalLight.h"
#include "LightSystem\SpotLight.h"


#include "DebugShapes.h"

void RenderEngine::DrawDirectionalLight( DirectionalLight& _Light )
{
	//m_pD3Dev->Clear(0,0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 255, 255), 1, 0);
	LPDIRECT3DSURFACE9 pOldRT;
	D3DSURFACE_DESC desc;
	m_pD3Dev->GetRenderTarget(0, &pOldRT );
	pOldRT->GetDesc(&desc);
	// Set the distance between a texel and pixel
	D3DXMATRIX inverse;
	D3DXMatrixInverse( &inverse, 0, &m_mMVP );
	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );

	//D3DXMATRIX scale;
	//D3DXMatrixScaling( &scale, (1.0 - 0.0) * 2.0f, (1.0 - 0.0) * 2.0f, 1.0f );
	//_matProj *= scale;

	//D3DXMATRIX translate;
	//D3DXMatrixTranslation( &translate, (0.0 * 2) - 1.0f, ( (0.0 * 2) - 1.0f ), 0.0f );
	//_matProj *= translate;
	if( DirectionalLightEffect )
	{
		_Light.ApplyLight( DirectionalLightEffect );
		DirectionalLightEffect->SetTechnique("RenderDirLight");
		unsigned passes(0);
		DirectionalLightEffect->Begin(&passes, 0);
		DirectionalLightEffect->SetMatrix("gWVP", &(_matProj));
		DirectionalLightEffect->SetVector( "gCameraPos", &D3DXVECTOR4( m_pCamera->GetViewPosition(), 1 ));
		DirectionalLightEffect->SetMatrix("gInvViewProj", &(inverse));
		DirectionalLightEffect->SetVector("gHalfPixel", 
			&D3DXVECTOR4(0.5f / desc.Width, 0.5f / desc.Height, 0, 1));

		DirectionalLightEffect->CommitChanges();	
		for(unsigned j(0); j<passes;++j)
		{
			DirectionalLightEffect->BeginPass(j);
			{	
				m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), postprocessMesh.m_uiBufferOffset, postprocessMesh.m_uiStride );
				m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
				m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, postprocessMesh.m_uiVertices, postprocessMesh.m_uiStartIndex, postprocessMesh.m_uiPrimCount );
			}
			DirectionalLightEffect->EndPass();
		}
		DirectionalLightEffect->End();
	}
	pOldRT->Release();
	//m_rtGEmissive.RevertTarget();
}

void RenderEngine::DrawPointLight( PointLight& _Light )
{
	//if( !ViewFrustum.infrustum( _Light.GetCollisionSphere() ) )
		//return;
	LPDIRECT3DSURFACE9 pOldRT;
	D3DSURFACE_DESC desc;
	m_pD3Dev->GetRenderTarget(0, &pOldRT );
	pOldRT->GetDesc(&desc);
	// Set the distance between a texel and pixel
	D3DXMATRIX inverse;
	D3DXMatrixInverse( &inverse, 0, &(m_mMVP));
//	_Light.SetAttenuation(&D3DXVECTOR3(0.00f, 0.0035f, 0.00006f)); 
	matrix4f mat;
	mat.make_identity();
	mat.axis_pos = _Light.GetFrame().GetWorldMat().axis_pos;
#if DEBUGSHAPES
	DebugShapes::RenderSphere( (D3DXMATRIX)mat.ma );
#endif
	if( PointLightEffect )
	{
		_Light.ApplyLight( PointLightEffect );
		unsigned passes(0);
		PointLightEffect->Begin(&passes, 0);
		HRESULT hr = PointLightEffect->SetMatrix("gWVP", &(((D3DXMATRIX)(_Light.GetFrame().GetWorldMat().ma)) * m_mMVP));
		PointLightEffect->SetVector( "gCameraPos", &D3DXVECTOR4( m_pCamera->GetViewPosition(), 1.0f ));
		PointLightEffect->SetMatrix("gInvViewProj", &(inverse));
		PointLightEffect->SetVector("gHalfPixel", 
			&D3DXVECTOR4(0.5f / desc.Width, 0.5f / desc.Height, 0, 1));

		PointLightEffect->CommitChanges();	
		for(unsigned j(0); j<passes;++j)
		{
			PointLightEffect->BeginPass(j);
			{	
				RenderNode* letMeSee = _Light.GetRenderNode();
				m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), letMeSee->renderData.m_uiBufferOffset, _Light.GetRenderNode()->renderData.m_uiStride );
				m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
				m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, _Light.GetRenderNode()->renderData.m_uiVertices, _Light.GetRenderNode()->renderData.m_uiStartIndex, _Light.GetRenderNode()->renderData.m_uiPrimCount );
				
			}
			PointLightEffect->EndPass();
		}
		PointLightEffect->End();
	}
	pOldRT->Release();
}

void RenderEngine::DrawSpotLight( SpotLight& _Light )
{
	//if( !ViewFrustum.infrustum( _Light.GetCollisionSphere() ) )
		//return;
	LPDIRECT3DSURFACE9 pOldRT;
	D3DSURFACE_DESC desc;
	m_pD3Dev->GetRenderTarget(0, &pOldRT );
	pOldRT->GetDesc(&desc);
	// Set the distance between a texel and pixel
	D3DXMATRIX inverse;
	D3DXMatrixInverse( &inverse, 0, &(m_mMVP));
	//_Light.SetAttenuation(&vec3f(0.15f, 0.0f, 0.0000125f), 15.0f);
	matrix4f mat;
	mat.make_identity();
	mat.axis_pos = _Light.GetPosition();
#if DEBUGSHAPES
	DebugShapes::RenderSphere( (D3DXMATRIX)mat.ma );
#endif
	if( SpotLightEffect )
	{
		_Light.ApplyLight( SpotLightEffect );
		unsigned passes(0);
		SpotLightEffect->Begin(&passes, 0);
		SpotLightEffect->SetMatrix("gWVP", &((D3DXMATRIX)(_Light.GetFrame().GetWorldMat().ma) * m_mMVP));
		SpotLightEffect->SetVector( "gCameraPos", &D3DXVECTOR4( m_pCamera->GetViewPosition(), 1.0f ));
		SpotLightEffect->SetMatrix("gInvViewProj", &(inverse));
		SpotLightEffect->SetVector("gHalfPixel", 
			&D3DXVECTOR4(0.5f / desc.Width, 0.5f / desc.Height, 0, 1));

		SpotLightEffect->CommitChanges();	
		for(unsigned j(0); j<passes;++j)
		{
			SpotLightEffect->BeginPass(j);
			{	
				m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), _Light.GetRenderNode()->renderData.m_uiBufferOffset, _Light.GetRenderNode()->renderData.m_uiStride );
				m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
				m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, _Light.GetRenderNode()->renderData.m_uiVertices, _Light.GetRenderNode()->renderData.m_uiStartIndex, _Light.GetRenderNode()->renderData.m_uiPrimCount );
				
			}
			SpotLightEffect->EndPass();
		}
		SpotLightEffect->End();
	}
	pOldRT->Release();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RasterTexture(): Draws a texture in screen space based on the scales passed in, used for menus. 
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::RasterTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom )
{
	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );

	D3DXMATRIX scale;
	D3DXMatrixScaling( &scale, (_right - _left) * 2.0f, (_bottom - _top) * 2.0f, 1.0f );
	_matProj *= scale;

	D3DXMATRIX translate;
	D3DXMatrixTranslation( &translate, (_left * 2) - 1.0f, ( (_top * 2) - 1.0f ), 0.0f );
	_matProj *= translate;

	TextureMeshEffect->SetTechnique("Basic");
	unsigned passes(0);
	TextureMeshEffect->Begin(&passes, 0);

	TextureMeshEffect->SetMatrix("gWVP", &(_matProj));
	TextureMeshEffect->SetTexture("tex1", _raster);
	TextureMeshEffect->CommitChanges();	
	for(unsigned j(0); j<passes;++j)
	{
		TextureMeshEffect->BeginPass(j);
		{	
			m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), screenMesh.m_uiBufferOffset, screenMesh.m_uiStride );
			m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
			m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
			m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, screenMesh.m_uiVertices, screenMesh.m_uiStartIndex, screenMesh.m_uiPrimCount );
		}
		TextureMeshEffect->EndPass();
	}
	TextureMeshEffect->End();
}

void RenderEngine::DrawFinalRender( LPDIRECT3DTEXTURE9 _raster )
{
	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );
	TextureMeshEffect->SetTechnique("Basic");
	unsigned passes(0);
	TextureMeshEffect->Begin(&passes, 0);

	TextureMeshEffect->SetMatrix("gWVP", &(_matProj));
	TextureMeshEffect->SetTexture("tex1", _raster);
	TextureMeshEffect->CommitChanges();	
	for(unsigned j(0); j<passes;++j)
	{
		TextureMeshEffect->BeginPass(j);
		{	
			m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), postprocessMesh.m_uiBufferOffset, postprocessMesh.m_uiStride );
			m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
			m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
			m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, postprocessMesh.m_uiVertices, postprocessMesh.m_uiStartIndex, postprocessMesh.m_uiPrimCount );
		}
		TextureMeshEffect->EndPass();
	}
	TextureMeshEffect->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawTexturedPlane(): Draws the texture passed in using the transform matrix. 
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::DrawTexturedPlane( D3DXMATRIX _transform, LPDIRECT3DTEXTURE9 _texture )
{
	TextureMeshEffect->SetTechnique("ForwardBasic");
	unsigned passes(0);
	TextureMeshEffect->Begin(&passes, 0);

	TextureMeshEffect->SetMatrix("gWVP", &(_transform * m_mMVP));
	TextureMeshEffect->SetTexture("tex1", _texture);
	TextureMeshEffect->CommitChanges();	
	for(unsigned j(0); j<passes;++j)
	{
		TextureMeshEffect->BeginPass(j);
		{	
			m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), screenMesh.m_uiBufferOffset, screenMesh.m_uiStride );
			m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
			m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
			m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, screenMesh.m_uiVertices, screenMesh.m_uiStartIndex, screenMesh.m_uiPrimCount );
		}
		TextureMeshEffect->EndPass();
	}
	TextureMeshEffect->End();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawCube(): Using a flat shader this function can draw the render node flat shaded, used for testing purposes.
//             Not deprecated can still be called as the shader is still loaded in INIT.           
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::DrawCube(RenderNode& pNode)
{	
	m_rtGEmissive.ActivateTarget( 0 );
	if(CubeEffect)
	{
		CubeEffect->SetTechnique("Basic");
		CubeEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));		
		CubeEffect->CommitChanges();		
		unsigned passes(0);
		CubeEffect->Begin(&passes, 0);
		for(unsigned i(0); i<passes; ++i)
		{
			CubeEffect->BeginPass(i);
			{
				m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, pNode.renderData.m_uiStride);
				m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
			}
			CubeEffect->EndPass();
		}
		CubeEffect->End();
	}
	m_rtGEmissive.RevertTarget();
}

void RenderEngine::DrawGBuffers( RenderNode& pNode )
{
	//Drawing call used to draw a flat shaded textured mesh
	if(GBufferEffect)
	{
		if( pNode.renderData.m_uiStride == sizeof(VERTEX_POSNORMTANUV) )
		{
			GBufferEffect->SetTechnique("GBuffers");
			unsigned passes(0);
			GBufferEffect->Begin(&passes, 0);			
			GBufferEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
			GBufferEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );

			if( pNode.m_pOwner->TestFlag( RE_UVS_NONE ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_NB ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_NX ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_NY ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_PB ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_PX ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_PY ) )
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else
				GBufferEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );

			GBufferEffect->SetTexture( "diffuseMap", (m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture));
			GBufferEffect->SetTexture( "normalMap", (m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[1]].m_lpTexture));
			GBufferEffect->SetTexture( "specularMap", (m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[2]].m_lpTexture));
			GBufferEffect->CommitChanges();	
			m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
			for(unsigned j(0); j<passes;++j)
			{
				GBufferEffect->BeginPass(j);
				{		
					m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, pNode.renderData.m_uiStride);
					m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
					m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
				}
				GBufferEffect->EndPass();
			}
			GBufferEffect->End();
		}
		else if( pNode.TestFlag( RE_ANIMATED ) )
		{
			if( AnimatedMeshEffect )
			{
				AnimatedMeshEffect->SetTechnique("SkinningGBuffers");

				// Loop through for each pass in the technique, it's usually one...Unless doing some transparency effect.
				unsigned int passes(0);
				AnimatedMeshEffect->Begin(&passes,0);
				for( UINT i = 0; i < passes; i++)
				{
					AnimatedMeshEffect->BeginPass(i);
					{
						D3DXMATRIX i;
						D3DXMatrixIdentity(&i);
						D3DXMATRIX WVP = (pNode.renderData.m_vTransform[pNode.renderData.m_vTransform.size()-1]  * m_mMVP);
						AnimatedMeshEffect->SetMatrix("gWVP", &WVP);
						GBufferEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[pNode.renderData.m_vTransform.size()-1] );										
						AnimatedMeshEffect->SetBool("hasTextureOne", true);
						GBufferEffect->SetTexture( "diffuseMap", (m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture));
						GBufferEffect->SetTexture( "normalMap", (m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[1]].m_lpTexture));
						GBufferEffect->SetTexture( "specularMap", (m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[2]].m_lpTexture));
						AnimatedMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture);

						AnimatedMeshEffect->SetMatrixArray("AnimPose", &pNode.renderData.m_vTransform[0], pNode.renderData.m_vTransform.size()-1);
						AnimatedMeshEffect->SetInt( "NumberOfMatrices", pNode.renderData.m_vTransform.size() );

						AnimatedMeshEffect->CommitChanges();

						m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
						m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetAnimVertexBuffer(), pNode.renderData.m_uiBufferOffset, pNode.renderData.m_uiStride);
						m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetAnimIndexBuffer());

						m_pD3Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount);

					}
					AnimatedMeshEffect->EndPass();
				}
				AnimatedMeshEffect->End();
			}
		}
	}
}

void RenderEngine::DrawDepth( RenderNode& pNode )
{
	//Drawing call used to draw a flat shaded textured mesh
	if(GBufferEffect)
	{
		if( pNode.renderData.m_uiStride == sizeof(VERTEX_POSNORMTANUV) )
		{
			for( unsigned int i = 0u; i < pNode.renderData.m_vTransform.size(); ++i ) 
			{
				GBufferEffect->SetTechnique("DepthPass");
				unsigned passes(0);
				GBufferEffect->Begin(&passes, 0);			
				GBufferEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[i] * m_mMVP));
				GBufferEffect->CommitChanges();	
				m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
				for(unsigned j(0); j<passes;++j)
				{
					GBufferEffect->BeginPass(j);
					{		
						m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, pNode.renderData.m_uiStride);
						m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
						m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
					}
					GBufferEffect->EndPass();
				}
				GBufferEffect->End();
			}
		}
		else if( pNode.TestFlag( RE_ANIMATED ) )
		{
			if( AnimatedMeshEffect )
			{
				AnimatedMeshEffect->SetTechnique("SkinningDepth");

				// Loop through for each pass in the technique, it's usually one...Unless doing some transparency effect.
				unsigned int passes(0);
				AnimatedMeshEffect->Begin(&passes,0);
				for( UINT i = 0; i < passes; i++)
				{
					AnimatedMeshEffect->BeginPass(i);
					{
						D3DXMATRIX i;
						D3DXMatrixIdentity(&i);
						D3DXMATRIX WVP = (pNode.renderData.m_vTransform[pNode.renderData.m_vTransform.size()-1]  * m_mMVP);
						AnimatedMeshEffect->SetMatrix("gWVP", &WVP);
						AnimatedMeshEffect->SetMatrixArray("AnimPose", &pNode.renderData.m_vTransform[0], pNode.renderData.m_vTransform.size()-1);
						AnimatedMeshEffect->SetInt( "NumberOfMatrices", pNode.renderData.m_vTransform.size() );

						AnimatedMeshEffect->CommitChanges();

						m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
						m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetAnimVertexBuffer(), pNode.renderData.m_uiBufferOffset, pNode.renderData.m_uiStride);
						m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetAnimIndexBuffer());

						m_pD3Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount);

					}
					AnimatedMeshEffect->EndPass();
				}
				AnimatedMeshEffect->End();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawTexturedMesh(): Uses the rendernode to draw a textured mesh, used for every object within the current game scene.
//                     The shader can be changed but please ask me first!
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::DrawTexturedMesh( RenderNode& pNode )
{
	//Drawing call used to draw a flat shaded textured mesh
	if(TextureMeshEffect)
	{
		if( pNode.TestFlag( RE_TRANSPARENT ) )
			TextureMeshEffect->SetTechnique( "TransparentTrap" );
		else if( pNode.TestFlag( RE_BLEND ) )
			TextureMeshEffect->SetTechnique( "BlendTrap" );			
		else if( pNode.TestFlag( RE_FORWARD ) )
			TextureMeshEffect->SetTechnique( "ForwardBasic" );
		else
			TextureMeshEffect->SetTechnique("Basic");

		unsigned passes(0);
		TextureMeshEffect->Begin(&passes, 0);
			
		TextureMeshEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
		TextureMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );
		TextureMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture );
		TextureMeshEffect->CommitChanges();	
		m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
		for(unsigned j(0); j<passes;++j)
		{
			TextureMeshEffect->BeginPass(j);
			{		
				m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, sizeof(VERTEX_POSNORMTANUV));
				m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
			}
			TextureMeshEffect->EndPass();
		}
		TextureMeshEffect->End();
	}
}

void RenderEngine::DrawScrollingTexturedMesh( RenderNode& pNode )
{
	//Drawing call used to draw a flat shaded textured mesh
	if(TextureMeshEffect)
	{
#if DEFERREDRENDER			//if( pNode.TestFlag( RE_TRANSPARENT ) || pNode.TestFlag( RE_BLEND ) )
		TextureMeshEffect->SetTechnique( "ForwardScrolling" );
#else
		TextureMeshEffect->SetTechnique("Scrolling");
#endif
		unsigned passes(0);
		TextureMeshEffect->Begin(&passes, 0);

		if( pNode.TestFlag( RE_UVS_NONE ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
		else if( pNode.TestFlag( RE_UVS_NB ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
		else if( pNode.TestFlag( RE_UVS_NX ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
		else if( pNode.TestFlag( RE_UVS_NY ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
		else if( pNode.TestFlag( RE_UVS_PB ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
		else if( pNode.TestFlag( RE_UVS_PX ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
		else if( pNode.TestFlag( RE_UVS_PY ) )
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
		else
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );

		TextureMeshEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
		TextureMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );
		TextureMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture );
		TextureMeshEffect->CommitChanges();	
		m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
		for(unsigned j(0); j<passes;++j)
		{
			TextureMeshEffect->BeginPass(j);
			{		
				m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, sizeof(VERTEX_POSNORMTANUV));
				m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
			}
			TextureMeshEffect->EndPass();
		}
		TextureMeshEffect->End();		
	}
}

void RenderEngine::DrawTexturedEnvironmentMesh( RenderNode& pNode )
{
	//Drawing call used to draw a flat shaded textured mesh
	if(TextureMeshEffect)
	{
		//CCollisionAABB aabb = pNode.renderData.m_vCollisions[0];

		//if( ViewFrustum.infrustum(  aabb ))
		{
			if( pNode.m_pOwner->TestFlag( RE_BURNTEX ) )
			{
				if( m_fBurnFactor <= -0.02f )
				{
					pNode.m_pOwner->ClearAllFlags();
					pNode.m_pOwner->AddFlags( RE_UVS_NONE | RE_FORWARD );
				}
				//Drawing call used to draw a flat shaded textured mesh
				if(TextureMeshEffect)
				{
#if DEFERREDRENDER
					TextureMeshEffect->SetTechnique( "ForwardDissolve" );
#else
					TextureMeshEffect->SetTechnique("Dissolve");
#endif
					unsigned passes(0);
					TextureMeshEffect->Begin(&passes, 0);

					TextureMeshEffect->SetTexture( "dissolveMap", m_pTextureMan->m_umTextureMap[m_pDissolveTexture].m_lpTexture );
					TextureMeshEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
					TextureMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.m_pOwner->renderData.m_uiTextureHandles[0]].m_lpTexture );
					HRESULT hr = TextureMeshEffect->SetTexture( "dissolveToMap", m_pTextureMan->m_umTextureMap[pNode.m_pOwner->renderData.m_uiTextureHandles[4]].m_lpTexture );

					if( pNode.m_pOwner->TestFlag( RE_UVS_NONE ) )
					{
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
						TextureMeshEffect->SetFloat( "dissolveFactor", 0.0f );
					}
					else
						TextureMeshEffect->SetFloat( "dissolveFactor", m_fBurnFactor );

					if( pNode.m_pOwner->TestFlag( RE_UVS_NB ) )
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
					else if( pNode.m_pOwner->TestFlag( RE_UVS_NX ) )
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
					else if( pNode.m_pOwner->TestFlag( RE_UVS_NY ) )
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
					else if( pNode.m_pOwner->TestFlag( RE_UVS_PB ) )
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
					else if( pNode.m_pOwner->TestFlag( RE_UVS_PX ) )
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
					else if( pNode.m_pOwner->TestFlag( RE_UVS_PY ) )
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
					else
						TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
					

					TextureMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );
					m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
					for(unsigned j(0); j<passes;++j)
					{
						TextureMeshEffect->BeginPass(j);
						{		
							m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, sizeof(VERTEX_POSNORMTANUV));
							m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
							m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
						}
						TextureMeshEffect->EndPass();
					}
					TextureMeshEffect->End();
				}
			}
			else
			{
				if( pNode.TestFlag( RE_TRANSPARENT ) )
					TextureMeshEffect->SetTechnique( "TransparentTrap" );
				else if( pNode.TestFlag( RE_BLEND ) )
					TextureMeshEffect->SetTechnique( "BlendTrap" );			
				else if( pNode.TestFlag( RE_FORWARD ) )
					TextureMeshEffect->SetTechnique( "ForwardBasic" );
				else
					TextureMeshEffect->SetTechnique("Basic");

				unsigned passes(0);
				TextureMeshEffect->Begin(&passes, 0);

				TextureMeshEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
				TextureMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );
				TextureMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture );
				TextureMeshEffect->CommitChanges();	
				m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
				for(unsigned j(0); j<passes;++j)
				{
					TextureMeshEffect->BeginPass(j);
					{		
						m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, sizeof(VERTEX_POSNORMTANUV));
						m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
						m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
					}
					TextureMeshEffect->EndPass();
				}
				TextureMeshEffect->End();
			}
		}
	}
}

void RenderEngine::DrawScrollingTexturedEnvironmentMesh( RenderNode& pNode )
{
	//Drawing call used to draw a flat shaded textured mesh
	if(TextureMeshEffect)
	{
		//CCollisionAABB aabb = pNode.renderData.m_vCollisions[0];

		//if( ViewFrustum.infrustum(  aabb ))
		{
#if DEFERREDRENDER			//if( pNode.TestFlag( RE_TRANSPARENT ) || pNode.TestFlag( RE_BLEND ) )
			TextureMeshEffect->SetTechnique( "ForwardScrolling" );
#else
			TextureMeshEffect->SetTechnique("Scrolling");
#endif
			unsigned passes(0);
			TextureMeshEffect->Begin(&passes, 0);

			if( pNode.TestFlag( RE_UVS_NONE ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.TestFlag( RE_UVS_NB ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.TestFlag( RE_UVS_NX ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.TestFlag( RE_UVS_NY ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.TestFlag( RE_UVS_PB ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.TestFlag( RE_UVS_PX ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.TestFlag( RE_UVS_PY ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );

			TextureMeshEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
			TextureMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );
			TextureMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture );
			TextureMeshEffect->CommitChanges();	
			m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
			for(unsigned j(0); j<passes;++j)
			{
				TextureMeshEffect->BeginPass(j);
				{		
					m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, sizeof(VERTEX_POSNORMTANUV));
					m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
					m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
				}
				TextureMeshEffect->EndPass();
			}
			TextureMeshEffect->End();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawLine(): Draws a line to the screen.
//             Currently does not work on ATI cards, change aim sight to cylinder.
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::DrawLine( D3DXVECTOR3 _start, D3DXVECTOR3 _end, D3DXCOLOR color )
{
	m_pCamera->SetLazerEndPoint(*(vec3f*)(&(_start)));
	m_pLineVertexs[0].m_position = _start;
	m_pLineVertexs[1].m_position = _end;
	m_pLineVertexs[1].m_color = color;
	m_pLineVertexs[0].m_color = color;
	m_rtGDiffuse.ActivateTarget(0);
	m_pDwordIndex[0] = 0;
	m_pDwordIndex[1] = 1;
	void* pInBuff;
	m_pLineVert->Lock(0,0,&pInBuff,0);
		memcpy( pInBuff, m_pLineVertexs, sizeof(VERTEX_POSCOLOR) * 2 );
	m_pLineVert->Unlock();
	m_pLineIndex->Lock(0,0,&pInBuff,0);
		memcpy( pInBuff, m_pDwordIndex, sizeof(DWORD) * 2 );
	m_pLineIndex->Unlock();
	m_pD3Dev->SetVertexDeclaration( m_pPosColor );
	if(CubeEffect)
	{
		CubeEffect->SetTechnique("Basic");
		CubeEffect->SetMatrix("gWVP", &(m_mMVP));		
		CubeEffect->CommitChanges();		
		unsigned passes(0);
		CubeEffect->Begin(&passes, 0);
		for(unsigned i(0); i<passes; ++i)
		{
			CubeEffect->BeginPass(i);
			{
				m_pD3Dev->SetStreamSource( 0, m_pLineVert, 0, sizeof(VERTEX_POSCOLOR) );
				m_pD3Dev->SetIndices(m_pLineIndex );
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_LINELIST, 0, 0, 2, 0, 1 );
			}
			CubeEffect->EndPass();
		}
		CubeEffect->End();
	}
	m_rtGDiffuse.RevertTarget();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DrawSmoothSkinnedMesh(): Draws an animated mesh to the screen.
//                       
// Returns: void
//                            
//
// Mod. Name: Bryan Sauter
// Mod. Date: 5/24/12
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RenderEngine::DrawSmoothSkinnedMesh( RenderNode& pNode )
{
	if( AnimatedMeshEffect )
	{
		AnimatedMeshEffect->SetTechnique("Skinning");

		// Loop through for each pass in the technique, it's usually one...Unless doing some transparency effect.
		unsigned int passes(0);
		AnimatedMeshEffect->Begin(&passes,0);
		for( UINT i = 0; i < passes; i++)
		{
			AnimatedMeshEffect->BeginPass(i);
			{
				D3DXMATRIX i;
				D3DXMatrixIdentity(&i);
				D3DXMATRIX WVP = (pNode.renderData.m_vTransform[pNode.renderData.m_vTransform.size()-1]  * m_mMVP);
				AnimatedMeshEffect->SetMatrix("gWVP", &WVP);
				AnimatedMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );												
				AnimatedMeshEffect->SetBool("hasTextureOne", true);
								
				AnimatedMeshEffect->SetTexture("tex1", m_pTextureMan->m_umTextureMap[pNode.renderData.m_uiTextureHandles[0]].m_lpTexture);

				AnimatedMeshEffect->SetMatrixArray("AnimPose", &pNode.renderData.m_vTransform[0], pNode.renderData.m_vTransform.size()-1);
				AnimatedMeshEffect->SetInt( "NumberOfMatrices", pNode.renderData.m_vTransform.size() );
								

				AnimatedMeshEffect->CommitChanges();

				m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
				m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetAnimVertexBuffer(), pNode.renderData.m_uiBufferOffset, pNode.renderData.m_uiStride);
				m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetAnimIndexBuffer());

				m_pD3Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0,0,pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount);

			}
			AnimatedMeshEffect->EndPass();
		}
		AnimatedMeshEffect->End();
	}
}

void RenderEngine::RasterTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom, float _fadeFactor )
{
	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );

	D3DXMATRIX scale;
	D3DXMatrixScaling( &scale, (_right - _left) * 2.0f, (_bottom - _top) * 2.0f, 1.0f );
	_matProj *= scale;

	D3DXMATRIX translate;
	D3DXMatrixTranslation( &translate, (_left * 2) - 1.0f, ( (_top * 2) - 1.0f ), 0.0f );
	_matProj *= translate;

	TextureMeshEffect->SetTechnique("Splash");
	unsigned passes(0);
	TextureMeshEffect->Begin(&passes, 0);

	TextureMeshEffect->SetMatrix("gWVP", &(_matProj));
	TextureMeshEffect->SetTexture("tex1", _raster);
	TextureMeshEffect->SetFloat( "fadeFactor", _fadeFactor );
	for(unsigned j(0); j<passes;++j)
	{
		TextureMeshEffect->BeginPass(j);
		{	
			m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), screenMesh.m_uiBufferOffset, screenMesh.m_uiStride );
			m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
			m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
			m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, screenMesh.m_uiVertices, screenMesh.m_uiStartIndex, screenMesh.m_uiPrimCount );
		}
		TextureMeshEffect->EndPass();
	}
	TextureMeshEffect->End();
}

void RenderEngine::RasterSourceRectTexture( LPDIRECT3DTEXTURE9 _raster, float _left, float _top, float _right, float _bottom, float _minX, float _maxX, float _minY, float _maxY, float _fadeFactor )
{
	D3DXMATRIX _matProj;
	D3DXMatrixIdentity( &_matProj );
	D3DXMatrixTranslation( &_matProj, -_minX, 0.0f, 0.0f );

	D3DXMATRIX scale;
	D3DXMatrixScaling( &scale, (_right - _left) * 2.0f, (_bottom - _top) * 2.0f, 1.0f );
	_matProj *= scale;

	D3DXMATRIX translate;
	D3DXMatrixTranslation( &translate, (_left * 2) - 1.0f, ( (_top * 2) - 1.0f ), 0.0f );
	_matProj *= translate;

	TextureMeshEffect->SetTechnique("SourceRect");
	unsigned passes(0);
	TextureMeshEffect->Begin(&passes, 0);
	TextureMeshEffect->SetMatrix("gWVP", &(_matProj));
	TextureMeshEffect->SetTexture("tex1", _raster);
	TextureMeshEffect->SetFloat( "fadeFactor", _fadeFactor );
	TextureMeshEffect->SetVector( "sourceRect", &D3DXVECTOR4( _minX, _maxX, _minY, _maxY ) );
	for(unsigned j(0); j<passes;++j)
	{
		TextureMeshEffect->BeginPass(j);
		{	
			m_pD3Dev->SetStreamSource( 0, MeshManager::GetInstance()->GetVertexBuffer(), screenMesh.m_uiBufferOffset, screenMesh.m_uiStride );
			m_pD3Dev->SetVertexDeclaration( m_pPosNormTanUVdecl );
			m_pD3Dev->SetIndices( MeshManager::GetInstance()->GetIndexBuffer() );
			m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, screenMesh.m_uiVertices, screenMesh.m_uiStartIndex, screenMesh.m_uiPrimCount );
		}
		TextureMeshEffect->EndPass();
	}
	TextureMeshEffect->End();
}

void RenderEngine::RenderBurningTexture( RenderNode& pNode )
{
	if( m_fBurnFactor <= -0.02f && !pNode.m_pOwner->TestFlag( RE_BURNTEX ) )
	{
		MeshManager::GetInstance()->AddNodeFlags( pNode.m_pOwner, RE_UVS_NONE | RE_BURNTEX );
	}
	//Drawing call used to draw a flat shaded textured mesh
	if(TextureMeshEffect)
	{
#if DEFERREDRENDER		//if( pNode.m_pOwner->TestFlag( RE_TRANSPARENT ) || pNode.m_pOwner->TestFlag( RE_BLEND ) )
			TextureMeshEffect->SetTechnique( "ForwardDissolve" );
#else
			TextureMeshEffect->SetTechnique("Dissolve");
#endif
		unsigned passes(0);
		TextureMeshEffect->Begin(&passes, 0);

		TextureMeshEffect->SetTexture( "dissolveMap", TextureManager::GetInstance()->m_umTextureMap[m_pDissolveTexture].m_lpTexture );
		TextureMeshEffect->SetMatrix("gWVP", &(pNode.renderData.m_vTransform[0] * m_mMVP));
		TextureMeshEffect->SetTexture("tex1", TextureManager::GetInstance()->m_umTextureMap[pNode.m_pOwner->renderData.m_uiTextureHandles[0]].m_lpTexture );
		TextureMeshEffect->SetTexture( "dissolveToMap", TextureManager::GetInstance()->m_umTextureMap[pNode.m_pOwner->renderData.m_uiTextureHandles[4]].m_lpTexture );

		if( pNode.m_pOwner->TestFlag( RE_BURNTEX ) )
		{
			TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
			TextureMeshEffect->SetFloat( "dissolveFactor", 0.0f );
		}
		else
		{
			if( pNode.m_pOwner->TestFlag( RE_UVS_NONE ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_NB ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_NX ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( -m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_NY ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, -m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_PB ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_PX ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( m_v2fScrollOffset.x, 0.0f, 0.0f, 0.0f ) );
			else if( pNode.m_pOwner->TestFlag( RE_UVS_PY ) )
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, m_v2fScrollOffset.y, 0.0f, 0.0f ) );
			else
				TextureMeshEffect->SetVector( "gTextureOffset", &D3DXVECTOR4( 0.0f, 0.0f, 0.0f, 0.0f ) );
			TextureMeshEffect->SetFloat( "dissolveFactor", m_fBurnFactor );
		}
		TextureMeshEffect->SetMatrix("gWorld", &pNode.renderData.m_vTransform[0] );
		m_pD3Dev->SetVertexDeclaration(pNode.renderData.m_d3dVertDecl);
		for(unsigned j(0); j<passes;++j)
		{
			TextureMeshEffect->BeginPass(j);
			{		
				m_pD3Dev->SetStreamSource(0, MeshManager::GetInstance()->GetVertexBuffer(), pNode.renderData.m_uiBufferOffset, sizeof(VERTEX_POSNORMTANUV));
				m_pD3Dev->SetIndices(MeshManager::GetInstance()->GetIndexBuffer());
				m_pD3Dev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pNode.renderData.m_uiVertices, pNode.renderData.m_uiStartIndex, pNode.renderData.m_uiPrimCount/* + 400*/ );
			}
			TextureMeshEffect->EndPass();
		}
		TextureMeshEffect->End();
	}
}