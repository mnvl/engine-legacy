
#define MAX_POINT_LIGHTS 5

struct PointLight
{
	float3 position;
	float4 color;
	float inv_attenuation;
};

uniform extern float4x4 view_world_projection_matrix;

uniform extern texture  diffuse_texture;
uniform extern texture  normal_map_texture;

uniform extern PointLight point_lights[MAX_POINT_LIGHTS];

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

	o.position_h = mul(float4(i.position, 1.0f), view_world_projection_matrix);
	o.position_w = i.position;
	o.texcoords = i.texcoords;
	o.tangent = i.tangent;
	o.binormal = i.binormal;
	o.normal = i.normal;

    return o;
}

float4 PS(uniform int point_lights_count, const VS_OUTPUT i) : COLOR
{
	float3 normal_tangent = normalize(2.0f * tex2D(NORMAL_MAP_SAMPLER, i.texcoords) - 1.0f);
    float3 normal_world = mul(normal_tangent, float3x3(i.tangent, i.binormal, i.normal));

	float4 color = 0.0f;

	for(int n = 0; n < point_lights_count; n++)
	{
		float3 delta = point_lights[n].position - i.position_w;
		float factor = pow(point_lights[n].inv_attenuation, length(delta));
		float power = saturate(dot(normalize(delta), normal_world) * factor);
		color = color + point_lights[n].color * power;
	}

	return color * tex2D(DIFFUSE_TEXTURE_SAMPLER, i.texcoords);
}

technique PL1
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(1);
    }
}

technique PL2
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(2);
    }
}

technique PL3
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(3);
    }
}

technique PL4
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(4);
    }
}

technique PL5
{
    pass p0
    {
        VertexShader = compile vs_2_0 VS();
        PixelShader  = compile ps_2_0 PS(5);
    }
}
