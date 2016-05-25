// globals
float4x4	gWVP;

//TODO:add variables for a scale value and a color
// The scale value is on a per object basis
// color is the color all the verts in this object should be
float scale;	
float4 color;
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
	//TODO:return the global color set already
	return c;
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
        CullMode = CCW;
    }
}