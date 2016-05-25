/*****************************************
* Texture.fx - handles a single texture
******************************************/
#include "CommonShader.fx"
#include "GBuffer.fx"

#ifndef _Animation_FX_
#define _Animation_FX_

// globals
//float4x4	gWVP;
texture		tex1;
uniform texture     fadeDiffuseMap;
uniform texture     fadeNormalMap;
uniform texture     fadeSpecularMap;
uniform float		fadeFactor;

float4x4	AnimPose[40];
int			NumberOfMatrices;

bool hasTextureOne;

float3 LightDirection;

/// The diffuse texture sampler 
sampler fadeDiffuseSampler = 
sampler_state
{
	Texture = <fadeDiffuseMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

/// The diffuse texture sampler 
sampler fadeNormalSampler = 
sampler_state
{
	Texture = <fadeNormalMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = WRAP;
	AddressV = WRAP;
};

/// The diffuse texture sampler 
sampler fadeSpecularSampler = 
sampler_state
{
	Texture = <fadeSpecularMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;	AddressU = WRAP;
	AddressV = WRAP;
};

// texture sampler
sampler		samp1
{
	Texture = <tex1>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};

// avilable filter options are NONE, POINT (Nearest Neighbor), NONE (average 2x2), and ANIOSOTROPIC. 
	
// Define a vertex shader output structure;
struct OutputVS
{
    float4 position		:POSITION0;
	float4 normal		:NORMAL0;
    float2 texCoord		:TEXCOORD0;
    float4 color		:COLOR0;
	float2 depth		:TEXCOORD1;
	float4 screenPos	:TEXCOORD3;
};

struct InputVS
{
	float3 position			: POSITION0;
	float3 normal			: NORMAL0;
	float3 tangent			: TANGENT0;
	float2 texCoord			: TEXCOORD0;
	float4 jointWeights		: BLENDWEIGHT0;
	float4 jointIndices		: BLENDINDICES0;
};

// Vertex shader -------------------------------------------------------------------------------------
OutputVS SmoothSkinningVS(InputVS inStruct)
{
	// output structure
	OutputVS outVS = (OutputVS)0;

	if( NumberOfMatrices <= 1 )
	{
		float4 curVert = float4(inStruct.position,1.0f);

		outVS.position = mul( curVert, gWVP);
	}
	else
	{

		int4 indexVector = inStruct.jointIndices;

		float weights[4] = (float[4])inStruct.jointWeights;
		int boneIndex[4] = (int[4])indexVector;

		float4 curVert = float4(inStruct.position,1.0f);
		float4 sumVert = float4(0.0,0.0,0.0,0.0);


		for( int j = 0; j < 4; j++)
		{
			if( weights[j] > 0.0f)
			{
				float4 manipulatedVert = mul(curVert, AnimPose[boneIndex[j]]) * weights[j];
				sumVert += manipulatedVert;
			}
		}

		outVS.position = mul( sumVert, gWVP);
	}

	outVS.normal = mul(float4(inStruct.normal, 0.0f), gWorld);
	outVS.texCoord = inStruct.texCoord;
	outVS.color = float4(1.0,1.0,1.0,1.0);

	outVS.depth.xy = outVS.position.zw;
	outVS.screenPos = outVS.position;
	outVS.screenPos.z = min( outVS.position.z, outVS.position.z );

    return outVS;
}

// Pixel Shader ---------------------------------------------------------------------------
UnLitFragOut SmoothSkinningPS(OutputVS inStruct)
{
	UnLitFragOut outCol;

	inStruct.screenPos.xy /= inStruct.screenPos.w;
	float2 texCoord = ((float2(inStruct.screenPos.x, -inStruct.screenPos.y) * 0.5) + 0.5);
	texCoord += gHalfPixel;

	float depth = tex2D( depthGBufferSampler, texCoord ).r;

	if( depth < inStruct.depth.x / inStruct.depth.y )
	{
		outCol.diffuse = tex2D( finalSampler, texCoord );
		discard;
	}

	outCol.diffuse = tex2D(samp1, inStruct.texCoord) * inStruct.color;
	
	if(outCol.diffuse.a < 0.9)
		discard;

	outCol.depth = float4(0,0,0,0);
	outCol.depth.r = inStruct.depth.x / inStruct.depth.y;
	outCol.normal = (normalize( inStruct.normal ) + 1.0f ) * 0.5f;

	return outCol;
}

// Vertex shader -------------------------------------------------------------------------------------
DepthVertexOut DepthAnimCamVertex(InputVS inStruct)
{
	// output structure
	DepthVertexOut depthOut;

	if( NumberOfMatrices <= 1 )
	{
		float4 curVert = float4(inStruct.position,1.0f);

		depthOut.position = mul(curVert, gWVP );
	}
	else
	{

		int4 indexVector = inStruct.jointIndices;

		float weights[4] = (float[4])inStruct.jointWeights;
		int boneIndex[4] = (int[4])indexVector;

		float4 curVert = float4(inStruct.position,1.0f);
		float4 sumVert = float4(0.0,0.0,0.0,0.0);


		for( int j = 0; j < 4; j++)
		{
			if( weights[j] > 0.0f)
			{
				float4 manipulatedVert = mul(curVert, AnimPose[boneIndex[j]]) * weights[j];
				sumVert += manipulatedVert;
			}
		}

		depthOut.position = mul(sumVert, gWVP );
	}

	depthOut.depth.xy = depthOut.position.zw;
	return depthOut;
}

float4 DepthAnimCamFragment( DepthVertexOut inVertex ) : COLOR
{
	return inVertex.depth.x / inVertex.depth.y;
}


// Vertex shader -------------------------------------------------------------------------------------
GBufferVertexOut GBuffersAnimVS(InputVS inStruct)
{
	// output structure
	GBufferVertexOut buffOut;

	if( NumberOfMatrices <= 1 )
	{
		float4 curVert = float4(inStruct.position,1.0f);

		buffOut.position = mul( curVert, gWVP);
	}
	else
	{

		int4 indexVector = inStruct.jointIndices;

		float weights[4] = (float[4])inStruct.jointWeights;
		int boneIndex[4] = (int[4])indexVector;

		float4 curVert = float4(inStruct.position,1.0f);
		float4 sumVert = float4(0.0,0.0,0.0,0.0);


		for( int j = 0; j < 4; j++)
		{
			if( weights[j] > 0.0f)
			{
				float4 manipulatedVert = mul(curVert, AnimPose[boneIndex[j]]) * weights[j];
				sumVert += manipulatedVert;
			}
		}

		buffOut.position = mul( sumVert, gWVP);
	}

	buffOut.normal = mul(float4(inStruct.normal, 0.0), gWorld );
	buffOut.texCoord = inStruct.texCoord;
	return buffOut;
}

GBufferFragOut GBuffersAnimFS(GBufferVertexOut inVert)
{
	GBufferFragOut fragOut;
	fragOut.normal = normalize( inVert.normal );
	fragOut.normal += 1;
	fragOut.normal *= 0.5;
	fragOut.diffuse = tex2D(diffuseSampler, inVert.texCoord);
	//fragOut.normal = tex2D(normalSampler, inVert.texCoord );
	fragOut.specular = tex2D(specularSampler, inVert.texCoord );
	//fragOut.emissive = 0.5f;
	fragOut.emissive = tex2D(emissiveSampler, inVert.texCoord );
	return fragOut;
}

GBufferFragOut GBuffersAnimFadeFS(GBufferVertexOut inVert)
{
	GBufferFragOut fragOut;

	float4 dissolveMapCol = tex2D(dissolveSampler, inVert.texCoord);

	if( dissolveMapCol.r < fadeFactor )
	{	
		fragOut.specular = tex2D(specularSampler, inVert.texCoord);
		if( fragOut.specular.x == 0.0f )
			fragOut.specular = float4(0,0,0, 1.0f );

		fragOut.diffuse = tex2D(diffuseSampler, inVert.texCoord);

		fragOut.normal = tex2D(normalSampler, inVert.texCoord);
		if( fragOut.normal.x <= 0.0f )
		{
			fragOut.normal = normalize( inVert.normal );
			fragOut.normal += 1;
			fragOut.normal *= 0.5;
		}
	}
	else
	{
		fragOut.specular = tex2D(fadeSpecularSampler, inVert.texCoord);
		if( fragOut.specular.x == 0.0f )
			fragOut.specular = float4(0,0,0, 1.0f );

		fragOut.diffuse = tex2D(fadeDiffuseSampler, inVert.texCoord);

		fragOut.normal = tex2D(fadeNormalSampler, inVert.texCoord);
		if( fragOut.normal.x <= 0.0f )
		{
			fragOut.normal = normalize( inVert.normal );
			fragOut.normal += 1;
			fragOut.normal *= 0.5;
		}
	}

	fragOut.emissive = tex2D(emissiveSampler, inVert.texCoord );

	return fragOut;
}

// ----------------------------------------------------------------------------------------
technique Skinning
{
    pass P0
    {
        VertexShader = compile vs_3_0 SmoothSkinningVS();
        PixelShader  = compile ps_3_0 SmoothSkinningPS();

		FillMode = Solid;
		//CullMode = CCW;
		CullMode = NONE;

		//ShadeMode = Gouraud;

		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = INVSrcAlpha;
    }

}

technique SkinningDepth
{
    pass P0
    {
        VertexShader = compile vs_3_0 DepthAnimCamVertex();
        PixelShader  = compile ps_3_0 DepthAnimCamFragment();

		FillMode = Solid;
		//CullMode = CCW;
		CullMode = NONE;

		//ShadeMode = Gouraud;

		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = INVSrcAlpha;
    }

}

technique SkinningGBuffers
{
	pass P0
	{
	    vertexShader = compile vs_3_0 GBuffersAnimVS();
        pixelShader  = compile ps_3_0 GBuffersAnimFS();

		FillMode = Solid;
		//CullMode = CCW;
		CullMode = NONE;

		//ShadeMode = Gouraud;

		//AlphaBlendEnable = true;
		//SrcBlend = SrcAlpha;
		//DestBlend = INVSrcAlpha;
	}
}

technique SkinningGBuffersFade
{
	pass P0
	{
	    vertexShader = compile vs_3_0 GBuffersAnimVS();
        pixelShader  = compile ps_3_0 GBuffersAnimFadeFS();
		FillMode = Solid;
        CullMode = CCW;
		//ZWriteEnable = FALSE;
	}
}

//technique Depth
//{
	//pass P0
	//{
		//VertexShader = compile vs_3_0
	//}
//}
//
#endif 