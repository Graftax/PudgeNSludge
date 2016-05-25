#ifndef DRAW_EDGE_FX
#define DRAW_EDGE_FX

#include "CommonShader.fx"

// Settings controlling the edge detection filter.
shared float EdgeWidth = 0.6;
shared float EdgeIntensity = 1.45;

// How sensitive should the edge detection be to tiny variations in the input data?
// Smaller settings will make it pick up more subtle edges, while larger values get
// rid of unwanted noise.
shared float NormalThreshold = 0.25;
shared float DepthThreshold = 0.005;

// How dark should the edges get in response to changes in the input data?
shared float NormalSensitivity = 1;
shared float DepthSensitivity = 10;

float4 DrawEdgeFS(TwoDVertexOut inVert) : COLOR0
{
	float4 sceneColor, normal1, normal2, normal3, normal4, diagonalDeltaNorm;
	float2 edgeOffset, texCoord;
	float depth1, depth2, depth3, depth4, normalDelta, depthDelta, edgeAmount;

	texCoord = inVert.texCoord + gHalfPixel;
    edgeOffset = EdgeWidth * (gHalfPixel * 2);
	sceneColor = tex2D(sourceSampler, texCoord);
	// Missing some stuff in this region...
	depth1 = tex2D( depthGBufferSampler, texCoord + float2(1,1)		* edgeOffset).r;
	depth2 = tex2D( depthGBufferSampler, texCoord + float2(1,-1)	* edgeOffset).r;
	depth3 = tex2D( depthGBufferSampler, texCoord + float2(-1,1)	* edgeOffset).r;
	depth4 = tex2D( depthGBufferSampler, texCoord + float2(-1,-1)	* edgeOffset).r;

	normal1 = tex2D( normalGBufferSampler, texCoord + float2(1,1)	* edgeOffset);
	normal2 = tex2D( normalGBufferSampler, texCoord + float2(1,-1)	* edgeOffset);
	normal3 = tex2D( normalGBufferSampler, texCoord + float2(-1,1)	* edgeOffset);
	normal4 = tex2D( normalGBufferSampler, texCoord + float2(-1,-1)	* edgeOffset);

	//1 n 4
	//2 n 3
	diagonalDeltaNorm = (abs(normal1 - normal4)) + (abs(normal2 - normal3));
	normalDelta = dot(diagonalDeltaNorm.xyz, 1.0);
	normalDelta -= NormalThreshold;
	normalDelta *= NormalSensitivity;
	normalDelta = saturate(normalDelta);

	depthDelta = (abs(depth1 - depth4)) + (abs(depth2 - depth3));
	depthDelta -= DepthThreshold;
	depthDelta *= DepthSensitivity;

	edgeAmount = ((normalDelta + depthDelta) * EdgeIntensity) * 0.5;

    // Apply the edge detection result to the main scene color.
    sceneColor *= (1 - edgeAmount);

    return float4(sceneColor.rgb, 1);
}

technique PostProcess
{
    pass P0
    {
        vertexShader = compile vs_3_0 TwoDVertex();
        pixelShader  = compile ps_3_0 DrawEdgeFS();
    }
}

#endif