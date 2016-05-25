#ifndef DOWNSAMPLE_FX
#define DOWNSAMPLE_FX

#include "2DTransform.fx"

float4 DownFilterX4FS(TwoDVertexOut inVert) : COLOR0
{
    float4 finalColor;
    float2 texCoord = inVert.texCoord + gHalfPixel;
    finalColor = 0;
    for (int i = 0; i < 16; i++)
    {
        finalColor += tex2D( sourceSampler, texCoord + TexelCoordsDownFilterX4[i].xy );
    }
    return finalColor / 16;
}

technique PostProcess
{
    pass P0
	<
		float fScaleX = 0.25f;
		float fScaleY = 0.25f;
	>
    {
        vertexShader = compile vs_2_0 TwoDVertexWithTransform();
        pixelShader  = compile ps_2_0 DownFilterX4FS();
        ZEnable = false;
    }
}

#endif