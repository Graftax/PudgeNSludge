/*****************************************
* Texture.fx - handles a single texture
******************************************/

// globals
float4x4	gWVP;
texture		tex1;
texture		tex2;
texture		tex3;

bool hasTextureOne;
bool hasTextureTwo;
bool hasTextureThree;


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
    float4 posH:POSITION0;
    float2 texCoord0:TEXCOORD0;
    float4 color:COLOR0;
};

struct InputVS
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float2 texCoord : TEXCOORD0;
	float4 jointWeights : BLENDWEIGHT0;
	float4 jointIndices : BLENDINDICES0;
};

// Vertex shader -------------------------------------------------------------------------------------
OutputVS TransformVS(InputVS inStruct)
{
	// output structure
	OutputVS outVS;
	outVS.posH = mul(float4(inStruct.position, 1.0f), gWVP);

	outVS.texCoord0 = inStruct.texCoord;
	outVS.color = float4(1.0,1.0,1.0,1.0);//diffuse;
    return outVS;
}

// Pixel Shader ---------------------------------------------------------------------------
float4 TransformPS(OutputVS inStruct):COLOR
{
	float4 OutPutColor = inStruct.color;

	if( hasTextureOne && hasTextureTwo && hasTextureThree)
	{
		float4 texColA = tex2D(samp1, inStruct.texCoord0);	
		float4 texColB = tex2D(samp2, inStruct.texCoord0);		
		float4 texColC = tex2D(samp3, inStruct.texCoord0);	
		
		// Dont want to blend color from a normal map and spec map.
		OutPutColor = texColA * inStruct.color;
	}
	else if( hasTextureOne )
	{
		float4 texColA = tex2D(samp1, inStruct.texCoord0);
		OutPutColor = texColA * inStruct.color;
	}

	return OutPutColor;

}

// ----------------------------------------------------------------------------------------
technique Basic
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformPS();

		//FillMode = Wireframe;
		CullMode = NONE;

		/*AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = INVSrcAlpha;*/
    }

}