#ifndef DEPTH_OF_FIELD_FX
#define DEPTH_OF_FIELD_FX

#include "CommonShader.fx"
#include "GBuffer.fx"

shared float gFocusDistance;
float gFocusRange = 0.3f;

/// A texture that should contain an in focus version of the scene
texture sharpTexture;
/// The diffuse texture sampler used to reference sharpTexture
sampler sharpSampler = 
sampler_state
{
	Texture = <sharpTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = WRAP;
	AddressV = WRAP;
};

float4 DepthOfFieldFS(TwoDVertexOut inVert) : COLOR0
{
	inVert.texCoord += gHalfPixel;
	float4 sharpColor, blurColor, finalColor;
	float depth, camDepth, blur;
	float2 texCoord = inVert.texCoord + gHalfPixel;
	depth = tex2D( depthGBufferSampler, inVert.texCoord ).r;
	camDepth = gPlaneY / (gPlaneX + depth );
	sharpColor = tex2D( sharpSampler, inVert.texCoord );
	blurColor = tex2D( sourceSampler, inVert.texCoord );
	blur = saturate(abs( ((gPlaneY / gPlaneX + depth) - gFocusDistance) / gFocusRange));
	finalColor = lerp(sharpColor, blurColor, blur );
	return finalColor;
}

technique PostProcess
{
    pass P0
    {
        vertexShader = compile vs_2_0 TwoDVertex();
        pixelShader  = compile ps_2_0 DepthOfFieldFS();
    }
}

#endif