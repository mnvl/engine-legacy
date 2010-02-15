
#include "point_light.fx"

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

technique StaticModel_PointLights5
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
		PixelShader = compile ps_2_0 PS(5);
    }
}
