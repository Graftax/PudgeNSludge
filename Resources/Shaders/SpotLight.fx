#include "CommonShader.fx"

#ifndef SPOTLIGHT_FX
#define SPOTLIGHT_FX

struct SpotLightVertexOut
{
	float4 position		: POSITION0;
	float4 screenPos	: TEXCOORD0;
};

SpotLightVertexOut DeferredSpotLightVS(float3 position : POSITION0, 
	float3 normal : NORMAL, float3 tangent : TANGENT0, float2 texCoord : TEXCOORD0)
{
	SpotLightVertexOut vertOut;
	vertOut.position = mul(float4(position, 1.0), gWVP );
	vertOut.position.z = min( vertOut.position.z, vertOut.position.w );
	vertOut.screenPos = vertOut.position;
	return vertOut;
}

float4 StencilFS(SpotLightVertexOut inVert) : COLOR0 
{
	return float4(1, 0, 0, 1);
}

float4 DeferredSpotLightFS(SpotLightVertexOut inVert) : COLOR0 
{
	inVert.screenPos.xy /= inVert.screenPos.w;

	float2 texCoord = ((float2(inVert.screenPos.x, -inVert.screenPos.y) * 0.5) + 0.5);
	texCoord += gHalfPixel;

	float4 ambient = SpotLight0.AmbientColor * tex2D(diffuseGBufferSampler, texCoord) * tex2D(diffuseGBufferSampler, texCoord).a;
	//SPECULAR
	float4 fragPos = float4( inVert.screenPos.xy, tex2D( depthGBufferSampler, texCoord ).r, 1.0 );
	fragPos = mul( fragPos, gInvViewProj );
	fragPos /= fragPos.w;

	float3 toLight = SpotLight0.Position.xyz - fragPos.xyz;

	float toLightLength = length(toLight);
	toLight = normalize(toLight);
	//DIFFUSE
	float4 fragNormal = (tex2D(normalGBufferSampler, texCoord ) * 2.0f) - 1.0f;	
	
	float4 diffuse = tex2D(diffuseGBufferSampler, texCoord) * saturate(dot(fragNormal.xyz, toLight )) * SpotLight0.DiffuseColor;
	
	float4 specSampColor = tex2D( specularGBufferSampler, texCoord );
	
	float specMod = pow( max( dot( reflect(-toLight, fragNormal.xyz), normalize(gCameraPos.xyz - fragPos.xyz)), 0.0f ), SpotLight0.SpecularPower );	
	specSampColor *= specMod;

	float d = distance( SpotLight0.Position, fragPos );

	float A = SpotLight0.Attenuation.x + SpotLight0.Attenuation.y * d + SpotLight0.Attenuation.z * d * d;

	float alpha = d / SpotLight0.Range;

    float damping = saturate( 1.0f - alpha * alpha );
    A *= damping;

	float spot = pow( max( dot( -toLight, SpotLight0.Direction ), 0.0f ), SpotLight0.Cutoff );
	//return spot;
	return (spot * ((/*A */ (ambient + diffuse)) + specSampColor)) * gBrightness;

	//float specMod = SpotLight0.SpecularIntensity * pow(saturate(dot(reflectVect, normalize(gCameraPos.xyz - fragPos.xyz))), SpotLight0.SpecularPower);
	//float3 reflectVect = reflect(-toLight, fragNormal.xyz);
	//float3 toCamera = normalize(gCameraPos.xyz - fragPos.xyz);
	//float nDotL = saturate(dot(fragNormal.xyz, toLight ));
	//fragNormal = (fragNormal * 2.0f) - 1.0f;
}

technique RenderSpotLightOutside
{
	pass P0
	{
		vertexShader = compile vs_3_0 DeferredSpotLightVS();
        pixelShader  = compile ps_3_0 DeferredSpotLightFS();
        
		CullMode = CCW;
		
		COLORWRITEENABLE = 0x0000000F;

		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable = FALSE;
    }

}

technique RenderSpotLightInside
{
	pass P0
	{
		vertexShader = compile vs_3_0 DeferredSpotLightVS();
        pixelShader  = compile ps_3_0 DeferredSpotLightFS();
        
		CullMode = CW;
		
		COLORWRITEENABLE = 0x0000000F;

		AlphaBlendEnable = TRUE;
		BlendOp = ADD;
		DestBlend = ONE;
		SrcBlend = ONE;

		ZWriteEnable = FALSE;
		ZEnable = FALSE;
    }

}














#endif