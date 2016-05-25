#ifndef GAUSSIAN_BLUR_FX
#define GAUSSIAN_BLUR_FX

#include "CommonShader.fx"

const shared float3 gBlurWeights;
const shared float gCenterBlurWeights;

struct BlurVertexOut
{
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
	float2 positiveOffsets[3] : TEXCOORD1;
    float2 negativeOffsets[3] : TEXCOORD4;
};

BlurVertexOut BlurVS(float3 position : POSITION0, float2 texCoord : TEXCOORD0,
	uniform float2 direction)
{
	BlurVertexOut outVert;
	outVert.position = float4(position, 1.0);
	outVert.texCoord = texCoord;
	//texCoord += gHalfPixel;
	//for( uint i = 1; i < 4; i++ )
	//{
		outVert.positiveOffsets[0] = texCoord + (((gHalfPixel * 2.0) * 1) * direction);
		outVert.negativeOffsets[0] = texCoord - (((gHalfPixel * 2.0) * 1) * direction);
		outVert.positiveOffsets[1] = texCoord + (((gHalfPixel * 2.0) * 2) * direction);
		outVert.negativeOffsets[1] = texCoord - (((gHalfPixel * 2.0) * 2) * direction);
		outVert.positiveOffsets[2] = texCoord + (((gHalfPixel * 2.0) * 3) * direction);
		outVert.negativeOffsets[2] = texCoord - (((gHalfPixel * 2.0) * 3) * direction);
	//}
	// Probably going to need to add some code here...

	return outVert;
}

float4 BlurPS(BlurVertexOut inVert) : COLOR0
{
	// These are to store the offset sample results
	float3x4 positiveSamples, negativeSamples;
	float4 color; // This will be the weighted sample for the center texel
	float2 texCoord = inVert.texCoord;
	texCoord += gHalfPixel;
	color = tex2D( sourceSampler, texCoord );
//	for( uint i = 0; i < 3; i++ )
//	{
		positiveSamples[0] = tex2D( sourceSampler, inVert.positiveOffsets[0] );
		negativeSamples[0] = tex2D( sourceSampler, inVert.negativeOffsets[0] );
		positiveSamples[1] = tex2D( sourceSampler, inVert.positiveOffsets[1] );
		negativeSamples[1] = tex2D( sourceSampler, inVert.negativeOffsets[1] );
		positiveSamples[2] = tex2D( sourceSampler, inVert.positiveOffsets[2] );
		negativeSamples[2] = tex2D( sourceSampler, inVert.negativeOffsets[2] );
//	}
	// There appears to be something missing in this region.
	float4 posBlurWeight = mul( gBlurWeights, positiveSamples );
	float4 negBlurWeight = mul( gBlurWeights, negativeSamples );
	color *= gCenterBlurWeights;

	return color + posBlurWeight + negBlurWeight;
}

#endif