#include "GBuffer.fx"

#ifndef DEFERRED_POINT_LIGHT_FX
#define DEFERRED_POINT_LIGHT_FX

struct PointLightVertexOut
{
	float4 position		: POSITION0;
	float4 screenPos	: TEXCOORD0;
};

float4 StencilFS(PointLightVertexOut inVert) : COLOR0 
{
	return float4(1, 0, 0, 1);
}

PointLightVertexOut DeferredPointLightVS(float3 position : POSITION0, 
	float3 normal : NORMAL, float3 tangent : TANGENT0, float2 texCoord : TEXCOORD0)
{
	PointLightVertexOut vertOut;
	vertOut.position = mul(float4(position, 1.0), gWVP );
	vertOut.position.z = min( vertOut.position.z, vertOut.position.w );
	vertOut.screenPos = vertOut.position;
	return vertOut;
}

float4 DeferredPointLightFS(PointLightVertexOut inVert) : COLOR 
{
	inVert.screenPos.xy /= inVert.screenPos.w;
	float2 texCoord = ((float2(inVert.screenPos.x, -inVert.screenPos.y) * 0.5) + 0.5);
	texCoord += gHalfPixel;
	float4 ambient = PointLight0.AmbientColor * tex2D(diffuseGBufferSampler, texCoord) * tex2D(diffuseGBufferSampler, texCoord).a;
	//SPECULAR
	float depth = tex2D( depthGBufferSampler, texCoord ).r;
	//return depth;
	float4 fragPos = float4( inVert.screenPos.xy, depth, 1.0 );
	fragPos = mul( fragPos, gInvViewProj );
	fragPos /= fragPos.w;
	float3 toLight = PointLight0.Position.xyz - fragPos.xyz;
	//return fragPos;
	float toLightLength = length(toLight);
	toLight /= toLightLength;
	//return (float4(toLight,1));
	//DIFFUSE
	float4 fragNormal = tex2D(normalGBufferSampler, texCoord );
	fragNormal = (fragNormal * 2.0f) - 1.0f;
	float nDotL = saturate(dot(fragNormal.xyz, toLight ));
	float4 diffuse = tex2D(diffuseGBufferSampler, texCoord) * nDotL * PointLight0.DiffuseColor;
	//return nDotL;
	float3 toCamera = normalize(gCameraPos.xyz - fragPos.xyz);
	float4 specSampColor = tex2D( specularGBufferSampler, texCoord );
	float3 reflectVect = normalize(reflect(-toLight, fragNormal.xyz));
	float specMod = PointLight0.SpecularIntensity * pow(saturate(dot(reflectVect, toCamera)), PointLight0.SpecularPower);
	specSampColor *= specMod;

	//Attenuation
	float attenuation = saturate(1.0 / (PointLight0.Attenuation.x + PointLight0.Attenuation.y * toLightLength + PointLight0.Attenuation.z * (toLightLength * toLightLength )));
	float alpha = toLightLength / PointLight0.Range;
	float damping = saturate( 1.0f - alpha * alpha );
	attenuation *= damping;
	//return attenuation;
	return (attenuation * (ambient + diffuse + specSampColor)) * gBrightness;
}

// The way to render a point light that the camera is outside of
technique RenderPointLightOutside
{
	pass P0
	{
		vertexShader = compile vs_3_0 DeferredPointLightVS();
        pixelShader  = compile ps_3_0 DeferredPointLightFS();
        
		CullMode = CW;
		
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable = FALSE;

		COLORWRITEENABLE = 0x0000000F;
    }
}

// The way to render a point light that the camera is within
technique RenderPointLightInside
{
	pass P0
	{
		vertexShader = compile vs_3_0 DeferredPointLightVS();
        pixelShader  = compile ps_3_0 DeferredPointLightFS();
        
		CullMode = CW;
		
		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable = FALSE;

		COLORWRITEENABLE = 0x0000000F;
    }
}

#endif