#ifndef TRANSFORM_FX
#define TRANSFORM_FX

#include "CommonShader.fx"

TwoDVertexOut TwoDVertexWithTransform(float3 position:POSITION0, float3 norm:NORMAL0, float3 tangent:TANGENT0, float2 uv0:TEXCOORD0)
{
	TwoDVertexOut outVert;

	outVert.position = mul(float4(position, 1), gWVP);
	outVert.texCoord = uv0;

	return outVert;
}

#endif