
float4 MaterialDiffuse : MATERIALDIFFUSE = {0.8f, 0.8f, 0.8f, 1.0f};

static const int MAX_MATRICES = 26;

float4x3    mWorldMatrixArray[MAX_MATRICES] : WORLDMATRIXARRAY;
float4x4    mViewProj : VIEWPROJECTION;

struct VS_INPUT
{
    float4  Pos             : POSITION;
    float3  Normal          : NORMAL;
    float4  Joints1         : BLENDINDICES;
    float4  Joints2         : BLENDINDICES;   
    float4  Weights1        : BLENDWEIGHT;
    float4  Weights2        : BLENDWEIGHT;
    float3  Tex0            : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
    float4  Diffuse : COLOR;
    float2  Tex0    : TEXCOORD0;
};

VS_OUTPUT VShade(VS_INPUT i, uniform int joints_count_1, uniform int joints_count_2)
{
    VS_OUTPUT   o;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector1 = D3DCOLORtoUBYTE4(i.Joints1);
    int4 IndexVector2 = D3DCOLORtoUBYTE4(i.Joints2);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray1[4] = (float[4])i.Weights1;
    float BlendWeightsArray2[4] = (float[4])i.Weights2;

    int   IndexArray1[4]        = (int[4])IndexVector1;
    int   IndexArray2[4]        = (int[4])IndexVector2;
    
    // calculate the pos/normal using the "normal" weights 
    // and accumulate the weights to calculate the last weight
    for (int index = 0;  index < joints_count_1; index++)
    {
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray1[index]]) * BlendWeightsArray1[index];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray1[index]]) * BlendWeightsArray1[index];
    }

    for (int index = 0;  index < joints_count_2; index++)
    {
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray2[index]]) * BlendWeightsArray2[index];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray2[index]]) * BlendWeightsArray2[index];
    }
    
    // transform position from world space into view and then projection space
    o.Pos = mul(float4(Pos.xyz, 1.0f), mViewProj);

    // normalize normals
    Normal = normalize(Normal);

    // Shade (Ambient + etc.)
    o.Diffuse.xyz = MaterialDiffuse.xyz;
    o.Diffuse.w = 1.0f;

    // copy the input texture coordinate through
    o.Tex0  = i.Tex0.xy;

    return o;
}

VertexShader vsArray[8] = { compile vs_2_0 VShade(1, 0), 
                            compile vs_2_0 VShade(2, 0),
                            compile vs_2_0 VShade(3, 0),
                            compile vs_2_0 VShade(4, 0),
                            compile vs_2_0 VShade(4, 1),
                            compile vs_2_0 VShade(4, 2),
                            compile vs_2_0 VShade(4, 3),
                            compile vs_2_0 VShade(4, 4),
                          };

technique t1
{
    pass p0
    {
        VertexShader = (vsArray[0]);
    }
}

technique t2
{
    pass p0
    {
        VertexShader = (vsArray[1]);
    }
}

technique t3
{
    pass p0
    {
        VertexShader = (vsArray[2]);
    }
}

technique t4
{
    pass p0
    {
        VertexShader = (vsArray[3]);
    }
}

technique t5
{
    pass p0
    {
        VertexShader = (vsArray[4]);
    }
}

technique t6
{
    pass p0
    {
        VertexShader = (vsArray[5]);
    }
}

technique t7
{
    pass p0
    {
        VertexShader = (vsArray[6]);
    }
}

technique t8
{
    pass p0
    {
        VertexShader = (vsArray[7]);
    }
}

