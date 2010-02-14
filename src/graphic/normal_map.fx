
struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float  specPower;
};

struct DirLight
{
	float4 ambient;
	float4 diffuse;
	float4 spec;
	float3 dirW;
};

uniform const int PROJECTION_PERSPECTIVE = 1;
uniform const int PROJECTION_ORTHO       = 2;

uniform extern int      projection_type;
uniform extern float3   camera_view_direction;
uniform extern float3   eye_position;

uniform extern float4x4 inverse_world_matrix;
uniform extern float4x4 view_world_projection_matrix;

uniform extern texture  diffuse_texture;
uniform extern texture  normal_map_texture;

uniform extern Material material;
uniform extern DirLight dir_light_1;

sampler TexS = sampler_state
{
	Texture = <diffuse_texture>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = WRAP;
    AddressV  = WRAP;
};

sampler NormalMapS = sampler_state
{
	Texture = <normal_map_texture>;
	MinFilter = ANISOTROPIC;
	MaxAnisotropy = 8;
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
    float4 posH      : POSITION0;
    float3 toEyeT    : TEXCOORD0;
    float3 lightDirT : TEXCOORD1;
    float2 tex0      : TEXCOORD2;
};

VS_OUTPUT NormalMapVS(const VS_INPUT i)
{
	VS_OUTPUT o;

	float3x3 world_to_tangent = transpose(float3x3(i.tangent, i.binormal, i.normal));

	if (projection_type == PROJECTION_PERSPECTIVE)
	{
		// Transform eye position to local space.
		float3 eyePosL = mul(float4(eye_position, 1.0f), inverse_world_matrix);

		// Transform to-eye vector to tangent space.
		float3 toEyeL = eyePosL - i.position;
		o.toEyeT = mul(toEyeL, world_to_tangent);
	}

	if (projection_type == PROJECTION_ORTHO)
	{
		o.toEyeT = mul(camera_view_direction, world_to_tangent);
	}

	// Transform light direction to tangent space.
	float3 lightDirL = mul(float4(dir_light_1.dirW, 0.0f), inverse_world_matrix).xyz;
	o.lightDirT  = mul(lightDirL, world_to_tangent);

	// Transform to homogeneous clip space.
	o.posH = mul(float4(i.position, 1.0f), view_world_projection_matrix);

	// Pass on texture coordinates to be interpolated in rasterization.
	o.tex0 = i.texcoords;

    return o;
}

float4 NormalMapPS(float3 toEyeT    : TEXCOORD0,
                   float3 lightDirT : TEXCOORD1,
                   float2 tex0      : TEXCOORD2) : COLOR
{
	// Interpolated normals can become unnormal--so normalize.
	toEyeT    = normalize(toEyeT);
	lightDirT = normalize(lightDirT);
	
	// Light vector is opposite the direction of the light.
	float3 lightVecT = -lightDirT;
	
	// Sample normal map.
	float3 normalT = tex2D(NormalMapS, tex0);
	
	// Expand from [0, 1] compressed interval to true [-1, 1] interval.
    normalT = 2.0f*normalT - 1.0f;
    
    // Make it a unit vector.
	normalT = normalize(normalT);
	
	// Compute the reflection vector.
	float3 r = reflect(-lightVecT, normalT);
	
	// Determine how much (if any) specular light makes it into the eye.
	float t  = pow(saturate(dot(r, toEyeT)), material.specPower);
	
	// Determine the diffuse light intensity that strikes the vertex.
	float s = saturate(dot(lightVecT, normalT));
	
	// If the diffuse light intensity is low, kill the specular lighting term.
	// It doesn't look right to add specular light when the surface receives 
	// little diffuse light.
	if(s <= 0.0f)
	     t = 0.0f;
	
	// Compute the ambient, diffuse and specular terms separatly. 
	float3 spec = t*(material.spec*dir_light_1.spec).rgb;
	float3 diffuse = s*(material.diffuse*dir_light_1.diffuse).rgb;
	float3 ambient = material.ambient*dir_light_1.ambient;
	
	// Get the texture color.
	float4 texColor = tex2D(TexS, tex0);
	
	// Combine the color from lighting with the texture color.
	float3 color = (ambient + diffuse)*texColor.rgb + spec;
	
	// Output the color and the alpha.
	return float4(color, material.diffuse.a*texColor.a);
}

technique NormalMapTech
{
    pass p0
    {
        VertexShader = compile vs_2_0 NormalMapVS();
        PixelShader  = compile ps_2_0 NormalMapPS();
    }
}
