
#if !defined(MAX_JOINT_MATRICES)
#define MAX_JOINT_MATRICES 54
#endif

float4x4 joint_matrix_array[MAX_JOINT_MATRICES] : WORLDMATRIXARRAY;

float3 ApplySkeletalAnimationToPosition(float3 p, uniform const int count, const int joints[8], const float weights[8])
{
	float3 o = 0;

	for (int index = 0; index < count; index++)
	{
		o += (float3) mul(float4(p, 1.0f), joint_matrix_array[joints[index]]) * weights[index];
	}

	return o;
}

float3 ApplySkeletalAnimationToVector(float3 v, uniform const int count, const int joints[8], const float weights[8])
{
	float3 o = 0;

	for (int index = 0; index < count; index++)
	{
		o += mul(v, (float3x3) joint_matrix_array[joints[index]]) * weights[index];
	}

	return o;
}

