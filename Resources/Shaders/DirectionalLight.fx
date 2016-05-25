#include "GBuffer.fx"

#ifndef DIRECTIONAL_LIGHT_FX
#define DIRECTIONAL_LIGHT_FX

struct DirLightVertexOut
{
	float4 position		: POSITION0;
	float2 texCoord		: TEXCOORD1;
	float4 screenPos	: TEXCOORD2;
};

DirLightVertexOut DeferredDirLightVS(float3 position : POSITION0, 
	float3 normal : NORMAL, float3 tangent : TANGENT0, float2 texCoord : TEXCOORD0)
{
	DirLightVertexOut vertOut;
	vertOut.position = vertOut.screenPos = mul(float4( position, 1.0 ), gWVP);
	vertOut.texCoord = texCoord;
	return vertOut;
}

float4 DeferredDirLightFS(DirLightVertexOut inVert) : COLOR 
{
	//AMBIENT
 	inVert.texCoord = inVert.texCoord + gHalfPixel;
	float4 ambient = DirLight.AmbientColor* tex2D(diffuseGBufferSampler, inVert.texCoord) * tex2D(diffuseGBufferSampler, inVert.texCoord).a;
	////DIFFUSE
	float4 fragNormal = tex2D(normalGBufferSampler, inVert.texCoord );
	fragNormal = (fragNormal * 2.0f) - 1.0f;
	float nDotL = saturate(dot(fragNormal.xyz, DirLight.Direction ));
	float4 diffuse = tex2D(diffuseGBufferSampler, inVert.texCoord) * nDotL * DirLight.DiffuseColor;
	////SPECULAR
	float depth = tex2D( depthGBufferSampler, inVert.texCoord ).r;
	float4 fragPos = float4( inVert.screenPos.xy, depth, 1.0 );	
	fragPos = mul( fragPos, gInvViewProj );
	fragPos /= fragPos.w;
	float3 toCamera = normalize(gCameraPos.xyz - fragPos.xyz);
	float4 specSampColor = tex2D( specularGBufferSampler, inVert.texCoord );
	float3 reflectVect = normalize(reflect(DirLight.Direction, fragNormal.xyz));
	float specMod = DirLight.SpecularIntensity * pow(saturate(dot(reflectVect, toCamera)), DirLight.SpecularPower);
	specSampColor *= specMod;

	return (ambient + diffuse + specSampColor) * gBrightness;
}

technique RenderDirLight
{
	pass P0
	{
		vertexShader = compile vs_3_0 DeferredDirLightVS();
        pixelShader  = compile ps_3_0 DeferredDirLightFS();
        
		CullMode = CCW;
		
		AlphaBlendEnable = FALSE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;
		ZWriteEnable = FALSE;
		ZEnable = FALSE;
    }

}

#endif