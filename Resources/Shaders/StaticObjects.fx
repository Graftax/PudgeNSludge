//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4x4	g_mWorldViewProjection;	// Camera's World * View * Projection matrix
float4x4	g_mWorld;				// World matrix for transforming each static object separately
float4		g_mColor;				// Vector for coloring each static object separately


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 vPosition : POSITION;
    float4 vColor    : COLOR0;
};


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  
};


//--------------------------------------------------------------------------------------
// This shader computes standard transforming and diffuse lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT DebugShapeVS(	float4 vPos : POSITION, 
							float3 vNormal : NORMAL)
{
    VS_OUTPUT Output;

    Output.vPosition =  mul(vPos,mul(g_mWorld, g_mWorldViewProjection));
    	
	Output.vColor = g_mColor;

    return Output;    
}


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color simply by passing
// through the interpolated vertex colors
//--------------------------------------------------------------------------------------
PS_OUTPUT DebugShapePS( VS_OUTPUT In ) 
{ 
    PS_OUTPUT Output;
    
    Output.RGBColor = In.vColor;
    
    return Output;
}


//--------------------------------------------------------------------------------------
// Renders a standard T&L scene with simple pixel shading
//--------------------------------------------------------------------------------------
technique DebugShape
{
    pass P1
    {          
        VertexShader = compile vs_3_0 DebugShapeVS(); 
        PixelShader  = compile ps_3_0 DebugShapePS(); 

		FillMode = Wireframe;
    }
}
