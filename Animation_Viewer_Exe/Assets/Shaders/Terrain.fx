/*****************************************
* Ground.fx - handles a single texture
******************************************/

//globals
float4x4	gWVP;
	
// Define a vertex shader output structure;
struct OutputVS
{
    float4 posH:POSITION0;
};

// Vertex shader -------------------------------------------------------------------------------------
OutputVS TransformVS(float3 position:POSITION0)
{
	// output structure
	OutputVS outVS;
	
	// create the transformed vertex position
	outVS.posH = mul(float4(position, 1.0f), gWVP);
	
    return outVS;


}

// Pixel Shader ---------------------------------------------------------------------------
float4 TransformPS():COLOR
{

		return float4(0.0f,0.5f,0.0f,1.0f);
}

// ----------------------------------------------------------------------------------------
technique Basic
{
    pass P0
    {
        vertexShader = compile vs_3_0 TransformVS();
        pixelShader  = compile ps_3_0 TransformPS();

      
		//ShadeMode = Flat;
		//ShadeMode = Gouraud;
        //FillMode = Wireframe;
        //CullMode = NONE;
    }
}