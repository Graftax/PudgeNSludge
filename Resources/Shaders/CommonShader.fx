#ifndef COMMON_SHADER_FX
#define COMMON_SHADER_FX


shared float4x4	gWVP;
shared float4x4	gWorld;
shared float4x4	gView;
shared float4x4	gInvViewProj;
shared float3 gCameraPos;
shared float2 gHalfPixel;
shared float gPlaneX;
shared float gPlaneY;
shared float gBrightness;

uniform float2 gTextureOffset;

shared float2 TexelCoordsDownFilterX4[16];

struct DepthVertexOut
{
	float4 position : POSITION;
	float2 depth	: TEXCOORD0;
};

struct GBufferFragOut
{
	float4 diffuse : COLOR0;
	float4 normal : COLOR1;
	float4 specular : COLOR2;
	float4 emissive : COLOR3;
};

/// An output vertex structure used in simple 2D shaders
struct TwoDVertexOut
{
	float4 position : POSITION0;
	float2 texCoord : TEXCOORD0;
};

struct UnLitFragOut
{
	float4 diffuse : COLOR0;
	float4 depth : COLOR1;
	float4 normal : COLOR2;
};

/// A simple 2D passthrough vertex function.
/// \param position - The position of the vertex
/// \param texCoord - The UV of the vertex
/// \return Returns the inputed data as a TwoDVertexOut structure
TwoDVertexOut TwoDVertex(float3 position : POSITION0, float2 texCoord : TEXCOORD0)
{
	TwoDVertexOut outVert;

	outVert.position = mul(float4( position, 1.0 ), gWVP);
	outVert.texCoord = texCoord;

	return outVert;
}
uniform texture     finalMap;
sampler		finalSampler
{
	Texture = <finalMap>;
	// texture filtering
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};

/// The diffuse texture to be applied 
shared texture ambientMap;
/// The diffuse texture sampler 
sampler ambientSampler = 
sampler_state
{
	Texture = <ambientMap>;
	MinFilter = NONE;
	MagFilter = NONE;
	MipFilter = NONE;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

shared texture dissolveMap;
sampler	dissolveSampler = 
sampler_state
{
	Texture = <dissolveMap>;
	// texture filtering
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;
	// texture address modes (available options are WRAP, MIRROR, BORDER, and CLAMP)
	AddressU  = WRAP;
	AddressV  = WRAP;
};
/// The collection of data needed to define a directional light
struct DirectionalLight
{
    bool		Enabled;
	float4x4	ViewProjection;
    float4		Direction;
    float4		DiffuseColor;
    float4		AmbientColor;
    float4		SpecularColor;
	float		SpecularPower;
    float		SpecularIntensity;
};

/// The collection of data needed to define a point light
struct PointLight
{
    bool		Enabled;
    float4		Position;
    float4		DiffuseColor;
    float4		AmbientColor;
	float3		Attenuation;
	float		Range;
    float4		SpecularColor;
    float		SpecularPower;
    float		SpecularIntensity;
};

struct SpotLight
{
    bool		Enabled;
	float4		Position;
	float4		Direction;
    float4		DiffuseColor;
    float4		AmbientColor;
	float4		SpecularColor;
	float		SpecularPower;
    float		SpecularIntensity;
	float3		Attenuation;
	float		Range;
	float		Exponent;
	float		Cutoff;
};

shared DirectionalLight DirLight;
shared PointLight PointLight0;
shared SpotLight SpotLight0;

/// The diffuse texture to be applied 
shared texture diffuseMap;
/// The diffuse texture sampler 
sampler diffuseSampler = 
sampler_state
{
	Texture = <diffuseMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};
//
/// The diffuse texture to be applied 
shared texture normalMap;
/// The diffuse texture sampler 
sampler normalSampler = 
sampler_state
{
	Texture = <normalMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = WRAP;
	AddressV = WRAP;
};

/// The diffuse texture to be applied 
shared texture specularMap;
/// The diffuse texture sampler 
sampler specularSampler = 
sampler_state
{
	Texture = <specularMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;	AddressU = WRAP;
	AddressV = WRAP;
};
//
///// The diffuse texture to be applied
shared texture emissiveMap;
/// The diffuse texture sampler
sampler emissiveSampler = 
sampler_state
{
	Texture = <emissiveMap>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;	
	AddressU = WRAP;
	AddressV = WRAP;
};

/// The texture diffuse result from GBuffers pass
shared texture diffuseGBuffer;
sampler diffuseGBufferSampler = 
sampler_state
{
	Texture = <diffuseGBuffer>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

/// The texture specular result from GBuffers pass
shared texture specularGBuffer;
sampler specularGBufferSampler = 
sampler_state
{
	Texture = <specularGBuffer>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

/// The texture normals result from GBuffers pass
shared texture normalGBuffer;
sampler normalGBufferSampler = 
sampler_state
{
	Texture = <normalGBuffer>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

/// The texture depth result from GBuffers pass
shared texture depthGBuffer;
sampler depthGBufferSampler = 
sampler_state
{
	Texture = <depthGBuffer>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

/// The source texture for post-process effects
texture sourceTexture;
sampler sourceSampler = 
sampler_state
{
	Texture = <sourceTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = POINT;

	AddressU = WRAP;
	AddressV = WRAP;
};





#endif //COMMON_SHADER_FX