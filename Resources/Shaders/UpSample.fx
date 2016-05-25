#ifndef UPSAMPLE_FX
#define UPSAMPLE_FX

#include "2DTransform.fx"

float4 UpFilterX4FS(TwoDVertexOut inVert) : COLOR0
{
	return tex2D(sourceSampler, inVert.texCoord + gHalfPixel);
}

technique PostProcess
{
    pass P0
	<
		float fScaleX = 4.0f;
		float fScaleY = 4.0f;
	>
    {
        vertexShader = compile vs_2_0 TwoDVertexWithTransform();
        pixelShader  = compile ps_2_0 UpFilterX4FS();
        ZEnable = false;
    }
}

#endif