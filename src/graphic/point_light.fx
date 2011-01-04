
#if !defined(MAX_POINT_LIGHTS)
#define MAX_POINT_LIGHTS 5
#endif

struct POINT_LIGHT
{
	float3 position;
	float3 color;
	float inv_attenuation;
};

uniform extern POINT_LIGHT point_lights[MAX_POINT_LIGHTS];

float3 GetPointLightIlluminationAtPosition(const int n, float3 position, float3 normal)
{
	float3 delta = point_lights[n].position - position;
	float power = saturate(point_lights[n].inv_attenuation * dot(delta, normal) / dot(delta, delta));
	return point_lights[n].color * power;
}

float3 GetPointLightsTotalIlluminationAtPosition(uniform const int count, float3 position, float3 normal)
{
	float3 color = 0.0f;

	for (int n = 0; n < count; n++)
	{
		color += GetPointLightIlluminationAtPosition(n, position, normal);
	}

	return color;
}
