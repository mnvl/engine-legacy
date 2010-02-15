
#include "point_light.fx"

float4x4 view_projection_matrix : VIEWPROJECTION;

uniform extern texture  diffuse_texture;
uniform extern texture  normal_map_texture;

sampler DIFFUSE_TEXTURE_SAMPLER = sampler_state
{
	Texture = <diffuse_texture>;
};

sampler NORMAL_MAP_SAMPLER = sampler_state
{
	Texture = <normal_map_texture>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

struct VS_INPUT
{
	float3 position  : POSITION0;
	float3 tangent   : TANGENT0;
	float3 binormal  : BINORMAL0;
	float3 normal    : NORMAL0;
	float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position_h : POSITION0;
	float3 position_w : TEXCOORD0;
    float2 texcoords  : TEXCOORD1;
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
	float3 normal     : TEXCOORD4;
};

VS_OUTPUT VS(const VS_INPUT i)
{
	VS_OUTPUT o;

	o.position_h = mul(float4(i.position, 1.0f), view_projection_matrix);
	o.position_w = i.position;
	o.texcoords = i.texcoords;
	o.tangent = i.tangent;
	o.binormal = i.binormal;
	o.normal = i.normal;

    return o;
}

float4 PS(uniform const int point_lights_count, const VS_OUTPUT i) : COLOR
{
	float3 normal_tangent = normalize(2.0f * tex2D(NORMAL_MAP_SAMPLER, i.texcoords) - 1.0f);
    float3 normal_world = mul(normal_tangent, float3x3(i.tangent, i.binormal, i.normal));

	float color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position_w, normal_world);

	return color * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}
