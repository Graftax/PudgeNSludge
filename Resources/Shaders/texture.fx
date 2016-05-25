#include "CommonShader.fx"
uniform texture		tex1;
uniform texture     dissolveToMap;
uniform float		fadeFactor;
uniform float		dissolveFactor;

float4 sourceRect;
sampler		samp1
{
	Texture = <tex1>;
	// texture filtering
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};

sampler		dissolveToSampler
{
	Texture = <dissolveToMap>;
	// texture filtering
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};
struct OutputVS
{
    float4 posH:POSITION0;
	float4 normal:TEXCOORD2;
    float2 texCoord0:TEXCOORD0;
	float2 depth:TEXCOORD1;
	float4 screenPos:TEXCOORD3;
};
OutputVS TransformVS(float3 position:POSITION0, float3 norm:NORMAL0, float3 tangent:TANGENT0, float2 uv0:TEXCOORD0)
{
	// output structure
	OutputVS outVS;
	
	// create the transformed vertex position
	outVS.posH = mul(float4(position, 1.0f), gWVP);	

	outVS.normal = mul(float4(norm, 0.0f), gWorld );
	outVS.depth.xy = outVS.posH.zw;
	// pass through the texture coordinates
	outVS.texCoord0 = uv0 + gHalfPixel;

	outVS.screenPos = outVS.posH;
	outVS.screenPos.z = min( outVS.posH.z, outVS.posH.z );
	
    return outVS;
}
OutputVS ScourceTransformVS(float3 position:POSITION0, float3 norm:NORMAL0, float3 tangent:TANGENT0, float2 uv0:TEXCOORD0)
{
	// output structure
	OutputVS outVS;
	
	// create the transformed vertex position

	outVS.posH = mul(float4(position, 1.0f), gWVP);	
	outVS.normal = mul(float4(norm, 0.0f), gWorld );
	outVS.depth.xy = outVS.posH.zw;
	// pass through the texture coordinates
	outVS.texCoord0 = uv0 + gHalfPixel;

	outVS.screenPos = outVS.posH;
	outVS.screenPos.z = min( outVS.posH.z, outVS.posH.z );
	
    return outVS;
}
OutputVS TransformScrollingVS(float3 position:POSITION0, float3 norm:NORMAL0, float3 tangent:TANGENT0, float2 uv0:TEXCOORD0)
{
	// output structure
	OutputVS outVS;
	
	// create the transformed vertex position
	outVS.posH = mul(float4(position, 1.0f), gWVP);	

	outVS.normal = mul(float4(norm, 0.0f), gWorld );
	outVS.depth.xy = outVS.posH.zw;
	// pass through the texture coordinates
	outVS.texCoord0 = uv0;
	outVS.texCoord0 += gTextureOffset;

	outVS.screenPos = outVS.posH;
	outVS.screenPos.z = min( outVS.posH.z, outVS.posH.z );
    return outVS;
}
float4 TransformPS(OutputVS inVert):COLOR
{	
	//inVert.screenPos.xy /= inVert.screenPos.w;
	//float2 texCoord = ((float2(inVert.screenPos.x, -inVert.screenPos.y) * 0.5) + 0.5);
	//texCoord += gHalfPixel;
	//float depth = tex2D( depthGBufferSampler, texCoord ).r;
	//if( depth < inVert.depth.x / inVert.depth.y )
		//return tex2D( finalSampler, texCoord );
	// determine the pixel to sampler from
    float4 texCol = tex2D(samp1, inVert.texCoord0);
    if( texCol.a <= 0.0f )
		discard;
    return texCol;
}
/*struct UnLitFragOut
{
	float4 diffuse : COLOR0;
	float4 depth : COLOR1;
	float3 normal : COLOR2;
};*/
UnLitFragOut ForwardTransformBlendPS( OutputVS inVert )
{
	UnLitFragOut outCol;

	inVert.screenPos.xy /= inVert.screenPos.w;
	float2 texCoord = ((float2(inVert.screenPos.x, -inVert.screenPos.y) * 0.5) + 0.5);
	texCoord += gHalfPixel;

	outCol.diffuse = tex2D( samp1, inVert.texCoord0 + gTextureOffset );
	float depth = tex2D( depthGBufferSampler, texCoord ).r;

	if( depth < inVert.depth.x / inVert.depth.y )
	{
		outCol.diffuse = tex2D( finalSampler, texCoord );
		discard;
	}

	if( outCol.diffuse.a <= 0.0f )
		discard;

	outCol.depth = float4(0,0,0,0);
	outCol.depth.r = inVert.depth.x / inVert.depth.y;
	outCol.normal = tex2D( normalGBufferSampler, texCoord);
	if( outCol.normal.r <= 0.05f )
		outCol.normal = (normalize( inVert.normal ) + 1.0f ) * 0.5f;

	return outCol;
}

UnLitFragOut ForwardTransformPS( OutputVS inVert )
{
	UnLitFragOut outCol;
	inVert.screenPos.xy /= inVert.screenPos.w;
	float2 texCoord = ((float2(inVert.screenPos.x, -inVert.screenPos.y) * 0.5) + 0.5);
	texCoord += gHalfPixel;

	outCol.diffuse = tex2D( samp1, inVert.texCoord0 + gTextureOffset );
	float depth = tex2D( depthGBufferSampler, texCoord ).r;

	if( depth < inVert.depth.x / inVert.depth.y )
	{
		outCol.diffuse = tex2D( finalSampler, texCoord );
		discard;
	}
	

	if( outCol.diffuse.a <= 0.0f )
		discard;

	outCol.depth = float4(0,0,0,0);
	outCol.depth.r = inVert.depth.x / inVert.depth.y;
	outCol.normal = (normalize( inVert.normal ) + 1.0f ) * 0.5f;

	return outCol;
}
float4 TransformSplashPS(float2 uv0:TEXCOORD0):COLOR
{
	// determine the pixel to sampler from
    float4 texCol = tex2D(samp1, uv0);
    return texCol * fadeFactor;
}

float4 SourceRectPS(float2 uv0:TEXCOORD0):COLOR
{
	if( uv0.x < sourceRect.x )
		discard;
	if( uv0.x > sourceRect.y )
		discard;
	if( uv0.y < sourceRect.z )
		discard;
	if( uv0.y > sourceRect.w )
		discard;

    float4 texCol = tex2D(samp1, uv0);

	if( texCol.a <= 0.05f )
		discard;

    return texCol;
}

float4 DissolvePS(float2 uv0:TEXCOORD0):COLOR
{
	// determine the pixel to sampler from
    float4 dissolveMapCol = tex2D(dissolveSampler, uv0 + gTextureOffset);
	float4 texCol = float4(1,1,1,1);
	if( dissolveMapCol.r < dissolveFactor )
	{
		texCol = tex2D( samp1, uv0 + gTextureOffset );
	}
	else if( dissolveMapCol.r < dissolveFactor + 0.02f )
	{
		texCol = float4(1,0,0,1);
	}
	else
	{
		texCol = tex2D( dissolveToSampler, uv0 );
	}
    return texCol;
}

UnLitFragOut ForwardDissolvePS( OutputVS inVert )
{
	UnLitFragOut outCol;
	// determine the pixel to sampler from
		inVert.screenPos.xy /= inVert.screenPos.w;
	float2 texCoord = ((float2(inVert.screenPos.x, -inVert.screenPos.y) * 0.5) + 0.5);
	texCoord += gHalfPixel;
    float4 dissolveMapCol = tex2D(dissolveSampler, inVert.texCoord0 + gTextureOffset);
	
	outCol.depth = float4(0,0,0,0);
	outCol.depth.r = inVert.depth.x / inVert.depth.y;
	outCol.normal = (normalize( inVert.normal ) + 1.0f ) * 0.5f;

	float depth = tex2D( depthGBufferSampler, texCoord ).r;

	if( depth < inVert.depth.x / inVert.depth.y )
	{
		outCol.diffuse = tex2D( finalSampler, texCoord );
		discard;
	}

	if( dissolveMapCol.r < dissolveFactor )
	{
		outCol.diffuse = tex2D( samp1, inVert.texCoord0 + gTextureOffset );
		if( outCol.diffuse.a <= 0.0f )
		discard;
	}
	else if( dissolveMapCol.r < dissolveFactor + 0.02f )
	{
		outCol.diffuse = float4(0.1,1,0.1,1);
	}
	else
	{
		outCol.diffuse = tex2D( dissolveToSampler, inVert.texCoord0 );
	}

    return outCol;
}
/*struct UnLitFragOut
{
	float4 diffuse : COLOR0;
	float4 depth : COLOR1;
	float3 normal : COLOR2;
};*/
//UnLitFragOut ForwardDissolvePS( OutputVS inVert )
//{
	//UnLitFragOut outCol(0);
//}

technique Basic
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformPS();
		//ShadeMode = Flat;
        //FillMode = Solid;
        CullMode = CCW;
    }
}

technique ForwardBasic
{
	pass P0
	{
		VertexShader = compile vs_3_0 TransformVS();
		PixelShader = compile ps_3_0 ForwardTransformPS();
		ShadeMode = Flat;
		FillMode = Solid;
		CullMode = CCW;
	}
}

technique TransparentTrap
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 ForwardTransformPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CW;

		AlphaBlendEnable = false;
    }
    pass P1
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 ForwardTransformBlendPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSRCAlpha;
    }
}

technique BlendTrap
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 ForwardTransformPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CW;
		AlphaBlendEnable = false;
    }
    pass P1
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 ForwardTransformBlendPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSRCAlpha;
    }
}

technique Splash
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformSplashPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
    }
}

technique Scrolling
{
	pass P0
	{
        vertexShader = compile vs_3_0 TransformScrollingVS();
        pixelShader  = compile ps_3_0 TransformPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
	}
}

technique ForwardScrolling
{
	pass P0
	{
        vertexShader = compile vs_3_0 TransformScrollingVS();
        pixelShader  = compile ps_3_0 ForwardTransformPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
	}
}

technique SourceRect
{
	pass P0
	{
        vertexShader = compile vs_3_0 ScourceTransformVS();
        pixelShader  = compile ps_3_0 SourceRectPS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
	}
}

technique Dissolve
{
	pass P0
	{
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 DissolvePS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
	}
}

technique ForwardDissolve
{
	pass P0
	{
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 ForwardDissolvePS();
		ShadeMode = Flat;
        FillMode = Solid;
        CullMode = CCW;
	}
}