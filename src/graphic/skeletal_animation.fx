
#if !defined(MAX_JOINT_MATRICES)
#define MAX_JOINT_MATRICES 54
#endif

float4x4 joint_matrix_array[MAX_JOINT_MATRICES] : WORLDMATRIXARRAY;

float3 ApplySkeletalAnimationToPosition(float3 p,
	uniform const int count1, const int4 joints1, const float4 weights1,
	uniform const int count2, const int4 joints2, const float4 weights2)
{
	float3 o;

	if (count1 > 0)
	{
		const int j1[4] = (const int[4]) joints1;
		const float w1[4] = (const float[4]) weights1;

		for (int index1 = 0; index1 < count1; index1++)
		{
			o += mul(float4(p, 1.0), joint_matrix_array[joints1[index1]]) * weights1[index1];
		}
	}

	if (count2 > 0)
	{
		const int j2[4] = (const int[4]) joints2;
		const float w2[4] = (const float[4]) weights2;

		for (int index2 = 0; index2 < count2; index2++)
		{
			o += mul(p, joint_matrix_array[joints2[index2]]) * weights2[index2];
		}
	}

	return o;
}

float3 ApplySkeletalAnimationToVector(float3 v,
	uniform const int count1, const int joints1, const float weights1,
	uniform const int count2, const int joints2, const float weights2)
{
	float3 o;
	return o;
}

