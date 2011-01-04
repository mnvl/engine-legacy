
#if defined(SKELETAL_ANIMATION_ENABLED)
#include "skeletal_animation.fx"
#endif

#if defined(LIGHTING_ENABLED)
#include "point_light.fx"
#endif

#if defined(NORMAL_MAP_ENABLED)
#include "normal_map.fx"
#endif

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

#if defined(VERTEX_TB_ENABLED)
    float3 tangent   : TANGENT0;
    float3 binormal  : BINORMAL0;
#endif

#if defined(VERTEX_N_ENABLED)
    float3 normal    : NORMAL0;
#endif

#if defined(VERTEX_JW_ENABLED)
    int4 joints1     : BLENDINDICES0;
    int4 joints2     : BLENDINDICES1;
    float4 weights1  : BLENDWEIGHT0;
	float4 weights2  : BLENDWEIGHT1;
#endif

    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position_h : POSITION;
	float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
#if defined(VERTEX_TB_ENABLED)
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
#endif
	float3 normal     : TEXCOORD4;
};


VS_OUTPUT VS(const VS_INPUT i)
{
    VS_OUTPUT o;

#if defined(VERTEX_JW_ENABLED)
	const int joints[8] = { i.joints1, i.joints2 };
	const float weights[8] = { i.weights1, i.weights2 };

	float3 position = ApplySkeletalAnimationToPosition(i.position, joints, weights);

    o.position_h = mul(float4(position, 1.0f), world_view_projection_matrix);
#else
    o.position_h = mul(float4(i.position, 1.0f), world_view_projection_matrix);
#endif

	o.position = o.position_h.xyz / o.position_h.w;

#if defined(VERTEX_JW_ENABLED) && defined(VERTEX_TB_ENABLED)
	float3 tangent = ApplySkeletalAnimationToVector(i.tangent, joints, weights);
	float3 binormal = ApplySkeletalAnimationToVector(i.binormal, joints, weights);

	o.tangent = normalize(mul(tangent, (float3x3) world_matrix));
	o.binormal = normalize(mul(binormal, (float3x3) world_matrix));
#endif

#if defined(VERTEX_JW_ENABLED) && defined(VERTEX_N_ENABLED)
	float3 normal = ApplySkeletalAnimationToVector(i.normal, joints, weights);
	o.normal = normalize(mul(normal, (float3x3) world_matrix));
#endif

#if !defined(VERTEX_JW_ENABLED) && defined(VERTEX_TB_ENABLED)
	o.tangent = normalize(mul(i.tangent, (float3x3) world_matrix));
	o.binormal = normalize(mul(i.binormal, (float3x3) world_matrix));
#endif

#if !defined(VERTEX_JW_ENABLED) && defined(VERTEX_N_ENABLED)
	o.normal = normalize(mul(i.normal, (float3x3) world_matrix));
#endif

    o.texcoords  = i.texcoords;

    return o;
}

float4 PS(const VS_OUTPUT_PTBNJWT i) : COLOR
{
#if defined(NORMAL_MAP_ENABLED)
	float3 normal = GetNormalMapNormal(i.texcoords, i.tangent, i.binormal, i.normal);
#else
	float3 normal = i.normal;
#endif

#if defined(LIGHTING_ENABLED)
	float4 color = float4(GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, normal), 1.0f);
#else
	float4 color = 1.0f;
#endif

	return color * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

technique TECH
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS();
    }
}

