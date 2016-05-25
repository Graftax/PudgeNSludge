#ifndef GAUSSIAN_BLURV_FX
#define GAUSSIAN_BLURV_FX

#include "PPBlur.fx"

float2 direction = float2(0.0f, 1.0f);

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