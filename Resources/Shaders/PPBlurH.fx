#ifndef GAUSSIAN_BLURH_FX
#define GAUSSIAN_BLURH_FX

#include "PPBlur.fx"

float2 direction = float2(1.0f, 0.0f);

technique PostProcess
{
	pass P0
	{
	    vertexShader = compile vs_3_0 BlurVS(direction);
        pixelShader  = compile ps_3_0 BlurPS();

        CullMode = CCW;
		ZWriteEnable = FALSE;
	}
}

#endif