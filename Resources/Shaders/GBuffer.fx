#include "CommonShader.fx"

#ifndef GBUFFER_FX
#define GBUFFER_FX

uniform texture     dissolveToMap;
uniform float		dissolveFactor;

struct GBufferVertexOut
{
	float4 position		: POSITION0;
	float4 normal		: TEXCOORD1;
	float2 texCoord		: TEXCOORD0;
};
sampler		dissolveToSampler
{
	Texture = <dissolveToMap>;
	// texture filtering
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};
//WRITE OUT THE GBUFFERS

GBufferVertexOut GBuffersVS(float3 position : POSITION0, 
	float3 normal : NORMAL, float3 tangent : TANGENT0, float2 texCoord : TEXCOORD0)
{
	GBufferVertexOut buffOut;
	buffOut.position = mul(float4(position, 1.0), gWVP);
	buffOut.normal = mul(float4(normal, 0.0), gWorld );
	buffOut.texCoord = texCoord;
	return buffOut;
}

GBufferFragOut GBuffersFS(GBufferVertexOut inVert)
{
	GBufferFragOut fragOut;
	fragOut.normal = tex2D(normalSampler, inVert.texCoord + gTextureOffset );
	if( fragOut.normal.x <= 0.0f )
	{
		fragOut.normal = normalize( inVert.normal );
		fragOut.normal += 1;
		fragOut.normal *= 0.5;
	}
	fragOut.diffuse = tex2D(diffuseSampler, inVert.texCoord + gTextureOffset);
	fragOut.specular = tex2D(specularSampler, inVert.texCoord + gTextureOffset );
	fragOut.emissive = tex2D(emissiveSampler, inVert.texCoord + gTextureOffset );
	if( fragOut.specular.x == 0.0f )
		fragOut.specular = float4(0,0,0, 1.0f );
	return fragOut;
}

GBufferFragOut GBuffersDisolveFS(GBufferVertexOut inVert)
{
	GBufferFragOut fragOut;

	float4 dissolveMapCol = tex2D(dissolveSampler, inVert.texCoord + (gTextureOffset * 2.0f) );
	
	fragOut.specular = tex2D(specularSampler, inVert.texCoord + gTextureOffset );
	if( fragOut.specular.x == 0.0f )
		fragOut.specular = float4(0,0,0, 1.0f );

	fragOut.diffuse = tex2D(diffuseSampler, inVert.texCoord + gTextureOffset);

	if( dissolveMapCol.r < dissolveFactor )
	{
		fragOut.normal = tex2D(normalSampler, inVert.texCoord + gTextureOffset );
		if( fragOut.normal.x <= 0.0f )
		{
			fragOut.normal = normalize( inVert.normal );
			fragOut.normal += 1;
			fragOut.normal *= 0.5;
		}
	}
	else if( dissolveMapCol.r < dissolveFactor + 0.02f )
	{
		fragOut.normal = normalize( inVert.normal );
		fragOut.normal += 1;
		fragOut.normal *= 0.5;
		fragOut.diffuse = float4(0.1,1,0.1,1);
	}
	else
	{
		fragOut.diffuse = tex2D( dissolveToSampler, inVert.texCoord + gTextureOffset );
		fragOut.normal = normalize( inVert.normal );
		fragOut.normal += 1;
		fragOut.normal *= 0.5;
	}
	fragOut.emissive = tex2D(emissiveSampler, inVert.texCoord + gTextureOffset );

	return fragOut;
}

technique GBuffers
{
	pass P0
	{
	    vertexShader = compile vs_3_0 GBuffersVS();
        pixelShader  = compile ps_3_0 GBuffersFS();

        CullMode = CCW;
		//ZWriteEnable = FALSE;
	}
}

technique GBuffersDissolve
{
	pass P0
	{
	    vertexShader = compile vs_3_0 GBuffersVS();
        pixelShader  = compile ps_3_0 GBuffersDisolveFS();

        CullMode = CCW;
		//ZWriteEnable = FALSE;
	}
}

///WRITE OUT THE DEPTH BUFFER
DepthVertexOut DepthCamVertex(float3 position : POSITION0, 
	float3 normal : NORMAL, float3 tangent : TANGENT0, float2 texCoord : TEXCOORD0)
{
	DepthVertexOut depthOut;
	depthOut.position = mul(float4(position,1.0), gWVP );
	depthOut.depth.xy = depthOut.position.zw;
	return depthOut;
}

float4 DepthCamFragment( DepthVertexOut inVertex ) : COLOR
{
	return inVertex.depth.x / inVertex.depth.y;
}


technique DepthPass
{
	pass P0
	{
		vertexShader	= compile vs_3_0 DepthCamVertex();
		pixelShader		= compile ps_3_0 DepthCamFragment();

		CullMode = CCW;
	}
}























#endif //GBUFFER_FX