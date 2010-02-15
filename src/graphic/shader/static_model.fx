
#include "point_light.fx"
#include "normal_map.fx"

float4x4 world_view_projection_matrix : VIEWPROJECTION;
float4x4 world_matrix : WORLD;

texture diffuse_texture;

sampler DIFFUSE_TEXTURE_SAMPLER = sampler_state
{
	Texture = <diffuse_texture>;
};

struct VS_INPUT
{
    float3 position  : POSITION;
    float3 normal    : NORMAL;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 position_h : POSITION;
    float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
	float3 normal     : TEXCOORD2;
};

struct VS_INPUT_NORMAL_MAP
{
    float3 position  : POSITION;
	float3 tangent   : TANGENT0;
	float3 binormal  : BINORMAL0;
	float3 normal    : NORMAL0;
    float2 texcoords : TEXCOORD0;
};

struct VS_OUTPUT_NORMAL_MAP
{
    float4 position_h : POSITION;
    float2 texcoords  : TEXCOORD0;
	float3 position   : TEXCOORD1;
	float3 tangent    : TEXCOORD2;
	float3 binormal   : TEXCOORD3;
	float3 normal     : TEXCOORD4;
};

VS_OUTPUT VS(const VS_INPUT i)
{
    VS_OUTPUT o;

    o.position_h = mul(float4(i.position, 1.0f), world_view_projection_matrix);
    o.texcoords  = i.texcoords;
	o.position   = mul(float4(i.position, 1.0f), world_matrix);
    o.normal     = mul(i.normal, world_matrix);

    return o;
}

float4 PS(uniform const int point_lights_count, const VS_OUTPUT i) : COLOR
{
	float4 color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, i.normal);
	return color * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

VS_OUTPUT_NORMAL_MAP VS_NormalMap(const VS_INPUT_NORMAL_MAP i)
{
    VS_OUTPUT_NORMAL_MAP o;

    o.position_h = mul(float4(i.position, 1.0f), world_view_projection_matrix);
    o.texcoords  = i.texcoords;
	o.position   = mul(float4(i.position, 1.0f), world_matrix);
    o.tangent    = mul(i.tangent,  world_matrix);
    o.binormal   = mul(i.binormal, world_matrix);
    o.normal     = mul(i.normal,   world_matrix);

    return o;
}

float4 PS_NormalMap(uniform const int point_lights_count, const VS_OUTPUT_NORMAL_MAP i) : COLOR
{
	float3 normal = GetNormalMapNormal(i.position, i.tangent, i.binormal, i.normal);
	float4 color = GetPointLightsTotalIlluminationAtPosition(point_lights_count, i.position, normal);
	return color * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

technique BASIC_TECH
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS(5);
    }
}

technique NORMAL_MAP_TECH
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS_NormalMap();
		PixelShader = compile ps_2_0 PS_NormalMap(5);
    }
}

