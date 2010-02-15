
float4x4 view_projection_matrix : VIEWPROJECTION;
float4x4 view_matrix : VIEW;

#include "skeletal_animation.fx"

struct VS_INPUT
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    int4 joints1     : BLENDINDICES0;
    int4 joints2     : BLENDINDICES1;
    float4 weights1  : BLENDWEIGHT0;
    float4 weights2  : BLENDWEIGHT1;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position  : POSITION;
	float3 normal    : NORMAL;
    float2 texcoords : TEXCOORD0;
};

VS_OUTPUT VS(uniform const int count1, uniform const int count2, const VS_INPUT i)
{
    VS_OUTPUT o;

	float3 position = ApplySkeletalAnimationToPosition(i.position, count1, i.joints1, i.weights1, count2, i.joints2, i.weights2);
	float3 normal = ApplySkeletalAnimationToVector(i.normal, count1, i.joints1, i.weights1, count2, i.joints2, i.weights2);

    o.position = mul(float4(position, 1.0f), view_projection_matrix);
    o.normal = mul(normal, view_matrix);
    o.texcoords  = i.texcoords;

    return o;
}

technique AnimatedModel_J1
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(1, 0);
    }
}

technique AnimatedModel_J2
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(2, 0);
    }
}

technique AnimatedModel_J3
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(3, 0);
    }
}

technique AnimatedModel_J4
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(4, 0);
    }
}

technique AnimatedModel_J5
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(4, 1);
    }
}

technique AnimatedModel_J6
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(4, 2);
    }
}

technique AnimatedModel_J7
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(4, 3);
    }
}

technique AnimatedModel_J8
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(4, 4);
    }
}
