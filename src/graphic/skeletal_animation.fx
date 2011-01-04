
#if !defined(MAX_JOINT_MATRICES)
#define MAX_JOINT_MATRICES 54
#endif

#if !defined(JOINTS_COUNT)
#define JOINTS_COUNT 8
#endif

float4x4 joint_matrix_array[MAX_JOINT_MATRICES] : WORLDMATRIXARRAY;

float3 ApplySkeletalAnimationToPosition(float3 p, const int joints[JOINTS_COUNT], const float weights[JOINTS_COUNT])
{
	float3 o = 0;

	for (int index = 0; index < JOINTS_COUNT; index++)
	{
		o += (float3) mul(float4(p, 1.0f), joint_matrix_array[joints[index]]) * weights[index];
	}

	return o;
}

float3 ApplySkeletalAnimationToVector(float3 v, const int joints[JOINTS_COUNT], const float weights[JOINTS_COUNT])
{
	float3 o = 0;

	for (int index = 0; index < JOINTS_COUNT; index++)
	{
		o += mul(v, (float3x3) joint_matrix_array[joints[index]]) * weights[index];
	}

	return o;
}

