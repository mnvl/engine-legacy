
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

VS_OUTPUT VShade(VS_INPUT i, uniform int NumBones)
{
    VS_OUTPUT   o;
    float3      Pos = 0.0f;
    float3      Normal = 0.0f;    
    float       LastWeight = 0.0f;
     
    // Compensate for lack of UBYTE4 on Geforce3
    int4 IndexVector = D3DCOLORtoUBYTE4(i.Joints1);

    // cast the vectors to arrays for use in the for loop below
    float BlendWeightsArray[4] = (float[4])i.Weights1;
    int   IndexArray[4]        = (int[4])IndexVector;
    
    // calculate the pos/normal using the "normal" weights 
    //        and accumulate the weights to calculate the last weight
    for (int iBone = 0; iBone < NumBones-1; iBone++)
    {
        LastWeight = LastWeight + BlendWeightsArray[iBone];
        
        Pos += mul(i.Pos, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(i.Normal, mWorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
    LastWeight = 1.0f - LastWeight; 

    // Now that we have the calculated weight, add in the final influence
    Pos += (mul(i.Pos, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight);
    Normal += (mul(i.Normal, mWorldMatrixArray[IndexArray[NumBones-1]]) * LastWeight); 
    
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

int CurNumBones = 2;
VertexShader vsArray[4] = { compile vs_2_0 VShade(1), 
                            compile vs_2_0 VShade(2),
                            compile vs_2_0 VShade(3),
                            compile vs_2_0 VShade(4)
                          };

//////////////////////////////////////
// Techniques specs follow
//////////////////////////////////////
technique t0
{
    pass p0
    {
        VertexShader = (vsArray[CurNumBones]);
    }
}

