
static const int MAX_MATRICES = 26;
float4x4 view_projection_matrix : VIEWPROJECTION;
float4x4 joint_matrix_array[MAX_MATRICES] : WORLDMATRIXARRAY;

struct VS_INPUT
{
    float3  position : POSITION;
    float3  normal : NORMAL;
    int4    joints1 : BLENDINDICES0;
    int4    joints2 : BLENDINDICES1;
    float4  weights1 : BLENDWEIGHT0;
    float4  weights2 : BLENDWEIGHT1;
    float2  texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  position : POSITION;
    float2  texcoords : TEXCOORD0;
};

VS_OUTPUT VS(const VS_INPUT i, uniform int count)
{
    VS_OUTPUT o;
    
    float3 position = { 0, 0, 0 };

	int indices1[4] = (int[4]) i.joints1;
    float weights1[4] = (float[4]) i.weights1;
    
    if (count <= 4)
    {
	    for (int index = 0;  index < count; index++)
	    {
			position += mul(i.position, joint_matrix_array[indices1[index]]) * weights1[index];
		}
	}
	else
	{
	    for (int index = 0;  index < 4; index++)
	    {
			position += mul(i.position, joint_matrix_array[indices1[index]]) * weights1[index];
		}

		int indices2[4] = (int[4]) i.joints2;
		float weights2[4] = (float[4]) i.weights2;

		for (int index = 0;  index < count - 4; index++)
	    {
			position += mul(i.position, joint_matrix_array[indices2[index]]) * weights2[index];
		}
	}

    o.position = mul(float4(position.xyz, 1), view_projection_matrix);
    o.texcoords  = i.texcoords.xy;

    return o;
}

VertexShader vertex_shaders_array[8] =
{
	compile vs_2_0 VS(1), 
    compile vs_2_0 VS(2),
    compile vs_2_0 VS(3),
    compile vs_2_0 VS(4),
    compile vs_2_0 VS(5),
    compile vs_2_0 VS(6),
    compile vs_2_0 VS(7),
    compile vs_2_0 VS(8),
};

technique t0
{
    pass p0
    {
        VertexShader = (vertex_shaders_array[7]);
    }
}