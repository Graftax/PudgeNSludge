// globals
float4x4	gWVP;
	
// Define a vertex shader output structure;
struct OutputVS
{
    float4 posH:POSITION0;
    float4 color:COLOR0;
};

// Vertex shader -------------------------------------------------------------------------------------
OutputVS TransformVS(float3 position:POSITION0, float4 col:COLOR0)
{
	// output structure
	OutputVS outVS;
	
	outVS.posH = mul(float4(position, 1.0f), gWVP);	
	outVS.color = col;
	
    return outVS;
}

// Pixel Shader ---------------------------------------------------------------------------
float4 TransformPS(float4 c:COLOR0):COLOR
{
	//return c;
    return float4(1.0, 0.0, 0.0, 1.0);
}

// ----------------------------------------------------------------------------------------
technique Basic
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformPS();
      
		ShadeMode = Flat;
        //FillMode = Wireframe;
        //CullMode = NONE;
    }
}