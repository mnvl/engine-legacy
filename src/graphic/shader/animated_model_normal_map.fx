
#include "skeletal_animation.fx"
#include "point_light.fx"
#include "normal_map.fx"

float4x4 world_view_projection_matrix : VIEWPROJECTION;
float4x4 world_matrix : WORLD;

uniform extern texture diffuse_texture;

sampler DIFFUSE_TEXTURE_SAMPLER = sampler_state
{
	Texture = <diffuse_texture>;
};

struct VS_INPUT
{
    float3 position  : POSITION;
	float3 tangent   : TANGENT0;
	float3 binormal  : BINORMAL0;
	float3 normal    : NORMAL0;
    int4 joints1     : BLENDINDICES0;
    int4 joints2     : BLENDINDICES1;
    float4 weights1  : BLENDWEIGHT0;
    float4 weights2  : BLENDWEIGHT1;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position_h : POSITION;
    float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
	float3 normal     : TEXCOORD4;
};

VS_OUTPUT VS(uniform const int count, const VS_INPUT i)
{
    VS_OUTPUT o;

	const int joints[8] = { i.joints1, i.joints2 };
	const float weights[8] = { i.weights1, i.weights2 };

	float3 position = ApplySkeletalAnimationToPosition(i.position, count, joints, weights);
    o.position_h = mul(float4(position, 1.0f), world_view_projection_matrix);

    o.texcoords  = i.texcoords;
	o.position = position;

	float3 binormal = ApplySkeletalAnimationToVector(i.binormal, count, joints, weights);
    o.binormal = mul(binormal, world_matrix);

	float3 tangent = ApplySkeletalAnimationToVector(i.tangent,  count, joints, weights);
    o.tangent = mul(tangent, world_matrix);

	float3 normal = ApplySkeletalAnimationToVector(i.normal,   count, joints, weights);
    o.normal = mul(normal, world_matrix);

    return o;
}

float4 PS(uniform const int point_lights_count, const VS_OUTPUT i) : COLOR
{
	float3 normal = GetNormalMapNormal(i.position, i.tangent, i.binormal, i.normal);
	float4 color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, normal);
	return color * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

technique AnimatedModel_Joints8_PointLights5_NormalMap
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS(8);
		PixelShader = compile ps_2_0 PS(5);
    }
}
