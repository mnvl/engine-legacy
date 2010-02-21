
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

struct VS_INPUT_PNT
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT_PNT
{
    float4 position_h : POSITION;
    float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
	float3 normal     : TEXCOORD2;
};

struct VS_INPUT_PTBNT
{
    float3 position  : POSITION;
	float3 tangent   : TANGENT0;
	float3 binormal  : BINORMAL0;
	float3 normal    : NORMAL0;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT_PTBNT
{
    float4 position_h : POSITION;
    float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
	float3 normal     : TEXCOORD4;
};

struct VS_INPUT_PNJWT
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    int4 joints1     : BLENDINDICES0;
    int4 joints2     : BLENDINDICES1;
    float4 weights1  : BLENDWEIGHT0;
    float4 weights2  : BLENDWEIGHT1;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT_PNJWT
{
    float4 position_h : POSITION;
    float2 texcoords  : TEXCOORD0;
    float3 position   : TEXCOORD1;
    float3 normal     : TEXCOORD2;
};

struct VS_INPUT_PTBNJWT
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

struct VS_OUTPUT_PTBNJWT
{
	float4 position_h : POSITION;
	float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
	float3 normal     : TEXCOORD4;
};

VS_OUTPUT_PNT VS_PNT(const VS_INPUT_PNT i)
{
    VS_OUTPUT_PNT o;

    o.position_h = mul(float4(i.position, 1.0f), world_view_projection_matrix);
    o.texcoords  = i.texcoords;
	o.position   = mul(float4(i.position, 1.0f), world_matrix);
	o.normal     = normalize(mul(i.normal, (float3x3) world_matrix));

    return o;
}

float4 PS_PNT(uniform const int point_lights_count, const VS_OUTPUT_PNT i) : COLOR
{
    float3 color = float4(GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, i.normal), 1.0f);
	return float4(color, 1.0f) * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

VS_OUTPUT_PTBNT VS_PTBNT(const VS_INPUT_PTBNT i)
{
    VS_OUTPUT_PTBNT o;

    o.position_h = mul(float4(i.position, 1.0f), world_view_projection_matrix);
    o.texcoords  = i.texcoords;
	o.position   = mul(float4(i.position, 1.0f), world_matrix);
	o.tangent    = normalize(mul(i.tangent, (float3x3) world_matrix));
	o.binormal   = normalize(mul(i.binormal, (float3x3) world_matrix));
	o.normal     = normalize(mul(i.normal, (float3x3) world_matrix));

    return o;
}

float4 PS_PTBNT(uniform const int point_lights_count, const VS_OUTPUT_PTBNT i) : COLOR
{
	float3 normal = GetNormalMapNormal(i.position, i.tangent, i.binormal, i.normal);
	float3 color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, normal);
	return float4(color, 1.0f) * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

VS_OUTPUT_PNJWT VS_PNJWT(uniform const int count, const VS_INPUT_PNJWT i)
{
    VS_OUTPUT_PNJWT o;

	const int joints[8] = { i.joints1, i.joints2 };
	const float weights[8] = { i.weights1, i.weights2 };

	float3 position = ApplySkeletalAnimationToPosition(i.position, count, joints, weights);
	float3 normal = ApplySkeletalAnimationToVector(i.normal, count, joints, weights);

    o.position_h = mul(float4(position, 1.0f), world_view_projection_matrix);
    o.texcoords  = i.texcoords;
	o.position   = mul(float4(position, 1.0f), world_matrix);
	o.normal     = normalize(mul(i.normal, (float3x3) world_matrix));

    return o;
}

float4 PS_PNJWT(uniform const int point_lights_count, const VS_OUTPUT_PNJWT i) : COLOR
{
    float3 color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, i.normal);
	return float4(color, 1.0f) * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

VS_OUTPUT_PTBNJWT VS_PTBNJWT(uniform const int count, const VS_INPUT_PTBNJWT i)
{
    VS_OUTPUT_PTBNJWT o;

	const int joints[8] = { i.joints1, i.joints2 };
	const float weights[8] = { i.weights1, i.weights2 };

	float3 position = ApplySkeletalAnimationToPosition(i.position, count, joints, weights);
    o.position_h = mul(float4(position, 1.0f), world_view_projection_matrix);

    o.texcoords  = i.texcoords;
	o.position = position;

	float3 binormal = ApplySkeletalAnimationToVector(i.binormal, count, joints, weights);
	o.binormal = normalize(mul(binormal, (float3x3) world_matrix));

	float3 tangent = ApplySkeletalAnimationToVector(i.tangent, count, joints, weights);
	o.tangent = normalize(mul(tangent, (float3x3) world_matrix));

	float3 normal = ApplySkeletalAnimationToVector(i.normal, count, joints, weights);
	o.normal = normalize(mul(normal, (float3x3) world_matrix));

    return o;
}

float4 PS_PTBNJWT(uniform const int point_lights_count, const VS_OUTPUT_PTBNJWT i) : COLOR
{
	float3 normal = GetNormalMapNormal(i.position, i.tangent, i.binormal, i.normal);
	float3 color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, normal);
	return float4(color, 1.0f) * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

technique TECH_PNT
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS_PNT();
		PixelShader = compile ps_2_0 PS_PNT(MAX_POINT_LIGHTS);
    }
}

technique TECH_PTBNT
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS_PTBNT();
		PixelShader = compile ps_2_0 PS_PTBNT(MAX_POINT_LIGHTS);
    }
}

technique TECH_PNJWT
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS_PNJWT(8);
		PixelShader = compile ps_2_0 PS_PNJWT(MAX_POINT_LIGHTS);
    }
}

technique TECH_PTBNJWT
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS_PTBNJWT(8);
		PixelShader = compile ps_2_0 PS_PTBNJWT(MAX_POINT_LIGHTS);
    }
}
