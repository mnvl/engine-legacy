
uniform extern texture  normal_map_texture;

sampler NORMAL_MAP_SAMPLER = sampler_state
{
	Texture = <normal_map_texture>;
	MinFilter = ANISOTROPIC;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

float3 GetNormalMapNormal(const float2 texcoords, const float3 tangent, const float3 binormal, const float3 normal)
{
	float3 normal_tangent = normalize(2.0f * tex2D(NORMAL_MAP_SAMPLER, texcoords) - 1.0f);
    return mul(normal_tangent, float3x3(normalize(tangent), normalize(binormal), normalize(normal)));
}
