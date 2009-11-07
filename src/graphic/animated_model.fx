
static const int MAX_MATRICES = 54;

float4x4 view_projection_matrix : VIEWPROJECTION;
float4x4 joint_matrix_array[MAX_MATRICES] : WORLDMATRIXARRAY;

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    int4 joints1 : BLENDINDICES0;
    int4 joints2 : BLENDINDICES1;
    float4 weights1 : BLENDWEIGHT0;
    float4 weights2 : BLENDWEIGHT1;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  position : POSITION;
    float2  texcoords : TEXCOORD0;
};

VS_OUTPUT VS(const VS_INPUT i)
{
    VS_OUTPUT o;
    
    float3 position = { 0, 0, 0 };

    int indices1[4] = (int[4]) i.joints1;
    float weights1[4] = (float[4]) i.weights1;
    
    for (int index1 = 0; index1 < 4; index1++)
    {
        position += mul(float4(i.position.xyz, 1.0), joint_matrix_array[indices1[index1]]) * weights1[index1];
    }

    int indices2[4] = (int[4]) i.joints2;
    float weights2[4] = (float[4]) i.weights2;

    for (int index2 = 0; index2 < 4; index2++)
    {
        position += mul(float4(i.position.xyz, 1.0), joint_matrix_array[indices2[index2]]) * weights2[index2];
    }

    o.position = mul(float4(position.xyz, 1.0), view_projection_matrix);
    o.texcoords  = i.texcoords.xy;

    return o;
}

technique t0
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
    }
}