/*****************************************
* Texture.fx - handles a single texture
******************************************/
#ifndef _Animation_FX_
#define _Animation_FX_

// globals
float4x4	gWVP;
float4x4	gWorld;
float4x4	gView;
float4x4	gProjection;
texture		tex1;
texture		tex2;
texture		tex3;

float4x4	InverseBindPose[29];
float4x4	CurrPose[29];

float4x4	AnimPose[35];

bool hasTextureOne;
bool hasTextureTwo;
bool hasTextureThree;

float3 LightDirection;


// texture sampler
sampler		samp1
{
	Texture = <tex1>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};

// texture sampler
sampler		samp2
{
	Texture = <tex2>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};

// texture sampler
sampler		samp3
{
	Texture = <tex3>;
	// texture filtering
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};
// avilable filter options are NONE, POINT (Nearest Neighbor), NONE (average 2x2), and ANIOSOTROPIC. 
	
// Define a vertex shader output structure;
struct OutputVS
{
    float4 position		:POSITION0;
    float2 texCoord		:TEXCOORD0;
    float4 color		:COLOR0;
};

struct InputVS
{
	float3 position			: POSITION0;
	float3 normal			: NORMAL0;
	float3 tangent			: TANGENT0;
	float2 texCoord			: TEXCOORD0;
	float4 jointWeights		: BLENDWEIGHT0;
	float4 jointIndices		: BLENDINDICES0;
};

// Vertex shader -------------------------------------------------------------------------------------
OutputVS SmoothSkinningVS(InputVS inStruct)
{
	// output structure
	OutputVS outVS = (OutputVS)0;

	 int4 indexVector = inStruct.jointIndices;

	float weights[4] = (float[4])inStruct.jointWeights;
	int boneIndex[4] = (int[4])indexVector;

	float4 curVert = float4(inStruct.position,1.0f);
	float4 sumVert = float4(0.0,0.0,0.0,0.0);


	for( int j = 0; j < 4; j++)
	{
		if( weights[j] > 0.0f)
		{
			float4 manipulatedVert = mul(curVert, AnimPose[boneIndex[j]]) * weights[j];
			sumVert += manipulatedVert;
		}
	}

	outVS.position = mul( sumVert, gWVP);
	outVS.texCoord = inStruct.texCoord;
	outVS.color = float4(1.0,1.0,1.0,1.0);

    return outVS;
}

// Pixel Shader ---------------------------------------------------------------------------
float4 SmoothSkinningPS(OutputVS inStruct) : COLOR
{
	float4 OutPutColor = inStruct.color;

	float4 texColA = tex2D(samp1, inStruct.texCoord);
	OutPutColor = texColA * inStruct.color;

	return OutPutColor;

}

// ----------------------------------------------------------------------------------------
technique Skinning
{
    pass P0
    {
        VertexShader = compile vs_3_0 SmoothSkinningVS();
        PixelShader  = compile ps_3_0 SmoothSkinningPS();

		//FillMode = Wireframe;
		CullMode = CCW;
		ShadeMode = Gouraud;

		/*AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = INVSrcAlpha;*/
    }

}

OutputVS SmoothSkinningLightingVS(InputVS inStruct)
{
	// output structure
	OutputVS outVS = (OutputVS)0;

	 int4 indexVector = inStruct.jointIndices;

	float weights[4] = (float[4])inStruct.jointWeights;
	int boneIndex[4] = (int[4])indexVector;

	float4 curVert = float4(inStruct.position,1.0f);
	float4 sumVert = float4(0.0,0.0,0.0,0.0);

	float4 curNorm = float4(inStruct.normal, 0.0f);
	float4 sumNorm = float4(0.0, 0.0, 0.0, 0.0);


	for( int j = 0; j < 4; j++)
	{
		if( weights[j] > 0.0f)
		{
			float4 manipulatedVert = mul(curVert, AnimPose[boneIndex[j]]) * weights[j];
			sumVert += manipulatedVert;
		}
	}

	for( int j = 0; j < 4; j++)
	{
		if( weights[j] > 0.0f)
		{
			float4 norm = mul(curNorm, AnimPose[boneIndex[j]]) * weights[j];
			sumNorm += norm;
		}
	}

	sumNorm = normalize(sumNorm);

	float3 normalW = mul( float4(sumNorm.xyz, 0.0f), gWorld);
	normalW = normalize(normalW);
	float4 lightAmount = saturate( dot(LightDirection, normalW));

	outVS.position = mul( sumVert, gWVP);
	outVS.texCoord = inStruct.texCoord;
	outVS.color = float4(1.0,1.0,1.0,1.0) * lightAmount;

    return outVS;
}

// Pixel Shader ---------------------------------------------------------------------------
float4 SmoothSkinningLightingPS(OutputVS inStruct) : COLOR
{
	float4 OutPutColor = inStruct.color;

	float4 texColA = tex2D(samp1, inStruct.texCoord);
	OutPutColor = texColA * inStruct.color;

	return OutPutColor;

}

technique SkinningAndLighting
{
	pass P0
	{
		VertexShader = compile vs_3_0 SmoothSkinningLightingVS();
		PixelShader = compile ps_3_0 SmoothSkinningLightingPS();

		ShadeMode = Gouraud;
		CullMode = CCW;
	}
}

OutputVS NoAnimationVS( InputVS inStruct)
{
	OutputVS outVS = (OutputVS)0;
	outVS.position = mul( float4(inStruct.position,1.0), gWVP);
	outVS.texCoord = inStruct.texCoord;
	outVS.color = float4(1.0,1.0,1.0,1.0);

	return outVS;
}

float4 NoAnimationPS(OutputVS inStruct) : COLOR
{
	float4 OutPutColor = inStruct.color;

	float4 texColA = tex2D(samp1, inStruct.texCoord);
	OutPutColor = texColA * inStruct.color;

	return OutPutColor;
}

technique Basic
{
	pass P0
	{
		VertexShader = compile vs_3_0 NoAnimationVS();
		PixelShader = compile ps_3_0 NoAnimationPS();

		CullMode = CCW;
	}
}

OutputVS LightingNoAnimationVS( InputVS inStruct)
{
	OutputVS outVS = (OutputVS)0;
	outVS.position = mul( float4(inStruct.position,1.0), gWVP);

	float3 normalW = mul( float4(inStruct.normal, 0.0f), gWorld);
	normalW = normalize(normalW);
	float4 lightAmount = saturate( dot(LightDirection, normalW));


	outVS.texCoord = inStruct.texCoord;

	outVS.color.rgb = float3(1.0,1.0,1.0) * lightAmount;
	outVS.color.a = 1.0f;

	return outVS;
}

float4 LightingNoAnimationPS(OutputVS inStruct) : COLOR
{
	float4 texCol = tex2D(samp1, inStruct.texCoord);

	return texCol * inStruct.color;
}

technique Lighting
{
	pass P0
	{
		VertexShader = compile vs_3_0 LightingNoAnimationVS();
		PixelShader = compile ps_3_0 LightingNoAnimationPS();

		ShadeMode = Gouraud;
		CullMode = CCW;
	}
}

#endif 