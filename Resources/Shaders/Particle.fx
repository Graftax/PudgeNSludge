// globals
#include "CommonShader.fx"

//float4x4	gWVP;
texture		Particle;
texture		AlphaRamp;
texture		ColorRamp;
texture		ScaleRamp;
texture		RotationRamp;

float		fScaleMin;
float		fScaleMax;

float		fRotationMin;
float		fRotationMax;

// texture sampler
sampler2D	samp1
{
	Texture = <Particle>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

// texture sampler
sampler2D	alphaSampler
{
	Texture = <AlphaRamp>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

sampler2D	colorSampler
{
	Texture = <ColorRamp>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

sampler2D	scaleSampler
{
	Texture = <ScaleRamp>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

sampler2D	rotationSampler
{
	Texture = <RotationRamp>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

// avilable filter options are NONE, POINT (Nearest Neighbor), NONE (average 2x2), and ANIOSOTROPIC. 
	
	struct InputVS
	{
		float3 position:POSITION0;
		float2 uv0:TEXCOORD0;
		float size:PSIZE0;
		float4 Color:COLOR0;
		float1 percentage:BLENDINDICES0;
	};

// Define a vertex shader output structure;
struct OutputVS
{
    float4 posH:POSITION0;
    float4 color:COLOR0;
    float2 texCoord0:TEXCOORD0;
    float1 size: PSIZE0;
	float1 index: COLOR1;
	float2 depth:COLOR2;
	float4 screenPos: COLOR3;
};

// Vertex shader -------------------------------------------------------------------------------------
OutputVS TransformVS( InputVS inputVS )
{
 	// output structure
  	OutputVS outVS;
  	
  	// create the transformed vertex position
  	outVS.posH = mul(float4(inputVS.position, 1.0f), gWVP);	
	//outVS.posH = float4( 0.0f, 0.0f, 0.0, 1.0f );
  	
  	// pass through the color
  	outVS.color = inputVS.Color;
  	
  	// pass through the texture coordinates
  	outVS.texCoord0 = inputVS.uv0;

	float4 scaleCol = tex2Dlod( scaleSampler, float4(0.0f, inputVS.percentage, 0.0, 0.0));

  	outVS.size = fScaleMin + ( scaleCol.r * ( fScaleMax - fScaleMin ) );
  	
	outVS.index = inputVS.percentage;

	outVS.depth.xy = outVS.posH.zw;

	outVS.screenPos = outVS.posH;
	outVS.screenPos.z = min(outVS.posH.z, outVS.posH.z);

    return outVS;

}

OutputVS ScreenTransVS( InputVS inputVS )
{
 	// output structure
  	OutputVS outVS;
  	
  	// create the transformed vertex position
  	//outVS.posH = mul(float4(inputVS.position, 1.0f), gWVP);	
	outVS.posH.x = inputVS.position.x;
	outVS.posH.y = inputVS.position.y;
	outVS.posH.z = 0.0f;
	outVS.posH.w = 1.0;
  	
  	// pass through the color
  	outVS.color = inputVS.Color;
  	
  	// pass through the texture coordinates
  	outVS.texCoord0 = inputVS.uv0;

	float4 scaleCol = tex2Dlod( scaleSampler, float4(0.0f, inputVS.percentage, 0.0, 0.0));

  	outVS.size = fScaleMin + ( scaleCol.r * ( fScaleMax - fScaleMin ) );
  	
	outVS.index = inputVS.percentage;

	outVS.depth.xy = outVS.posH.zw;

	outVS.screenPos = outVS.posH;
	outVS.screenPos.z = min(outVS.posH.z, outVS.posH.z);

    return outVS;
}

// magic in the geometry shader happens here! Boom!

// Pixel Shader ---------------------------------------------------------------------------
float4 TransformPS(float4 c:COLOR0, float2 uv0:TEXCOORD0, float4 rotation:COLOR1, float1 size: PSIZE0, float1 index: COLOR1,
	float2 depth:COLOR2, float4 screenPos: COLOR3 ):COLOR
{
	// determine the pixel to sampler from
	screenPos.xy /= screenPos.w;
	float2 screenCoord = ((float2(screenPos.x, -screenPos.y) * 0.5) + 0.5);
	screenCoord += gHalfPixel;

	float screenDepth = tex2D( depthGBufferSampler, screenCoord ).r;
	if( screenDepth < ( depth.x / depth.y ) )
		discard;
	// ROTATION
	float2 aUV = uv0 - float2( 0.5f, 0.5f );

	float4 rotCol = tex2D( rotationSampler, float2( 0.0f, index ) );

	float fRot = fRotationMin + rotCol.r * ( fRotationMax - fRotationMin );

	uv0.x = aUV.x * cos( fRot ) - aUV.y * sin( fRot );
	uv0.y = aUV.x * sin( fRot ) + aUV.y * cos( fRot );
	uv0 += float2( 0.5f, 0.5f );

	float4 texCol = tex2D(samp1, uv0);

	if( texCol.a < 0.01f )
		discard;

	float4 alphaCol = tex2D( alphaSampler, float2( 0.0f, index ) );

	c.a = alphaCol.r;

	float4 colorCol = tex2D( colorSampler, float2( 0.0f, index ) );

	c.r = colorCol.r;
	c.g = colorCol.g;
	c.b = colorCol.b;

	return texCol * c;
}

// ----------------------------------------------------------------------------------------
technique Basic
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformPS();
      
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSRCAlpha;
		//DestBlend = One;
		
		ZWriteEnable = false;
		
		PointSpriteEnable = true;
		PointScaleEnable = true;
		
		PointSize = 10.0f;
		PointSize_Min = 1.0f;
		PointSize_Max = 1000.0f;
		
		//NewSize = BaseSize * sqrt( 1/ (A + B*Dis + C*Dis*Dis) )
		PointScale_A = 1.0f;
		PointScale_B = 0.0f;
		PointScale_C = 0.0f;

    }
}

technique BasicHUD
{
	pass P0
	{
		vertexShader = compile vs_3_0 ScreenTransVS();
        pixelShader  = compile ps_3_0 TransformPS();
      
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSRCAlpha;
		//DestBlend = One;
		
		ZWriteEnable = false;
		
		PointSpriteEnable = true;
		PointScaleEnable = true;
		
		PointSize = 10.0f;
		PointSize_Min = 1.0f;
		PointSize_Max = 1000.0f;
		
		//NewSize = BaseSize * sqrt( 1/ (A + B*Dis + C*Dis*Dis) )
		PointScale_A = 1.0f;
		PointScale_B = 0.0f;
		PointScale_C = 0.0f;
	}
}

