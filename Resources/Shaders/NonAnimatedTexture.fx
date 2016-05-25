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


float4x4	AnimPose[40];

bool hasTextureOne;

float3 LightDirection;


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


	float4 curVert = float4(inStruct.position,1.0f);

	outVS.position = mul( curVert, gWVP);
	outVS.normal = mul(float4(inStruct.normal, 0.0f), gWorld);
	outVS.texCoord = inStruct.texCoord;
	outVS.color = float4(1.0,1.0,1.0,1.0);

    return outVS;
}

// Pixel Shader ---------------------------------------------------------------------------
float4 SmoothSkinningPS(OutputVS inStruct) : COLOR
{
	float4 OutPutColor = inStruct.color;

	float4 texColA = tex2D(samp1, inStruct.texCoord);
	OutPutColor = texColA * inStruct.color;
	
	if(OutPutColor.a < 0.3)
		discard;

	return OutPutColor;

}

// Vertex shader -------------------------------------------------------------------------------------
DepthVertexOut DepthAnimCamVertex(InputVS inStruct)
{
	// output structure
	DepthVertexOut depthOut;

	float4 curVert = float4(inStruct.position,1.0f);

	depthOut.position = mul(curVert, gWVP );
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

	float4 curVert = float4(inStruct.position,1.0f);

	buffOut.position = mul(curVert, gWVP );
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
	return fragOut;
}

// ----------------------------------------------------------------------------------------
technique Skinning
{
    pass P0
    {
        VertexShader = compile vs_3_0 SmoothSkinningVS();
        PixelShader  = compile ps_3_0 SmoothSkinningPS();

		//FillMode = Wireframe;
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

		//FillMode = Wireframe;
		//CullMode = CCW;
		CullMode = NONE;

		//ShadeMode = Gouraud;

		/*AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = INVSrcAlpha;*/
    }

}

technique SkinningGBuffers
{
	pass P0
	{
	    vertexShader = compile vs_3_0 GBuffersAnimVS();
        pixelShader  = compile ps_3_0 GBuffersAnimFS();

        //CullMode = CCW;
		CullMode = NONE;

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