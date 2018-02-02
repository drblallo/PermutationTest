__kernel void p_test
(
	__global const float* a,
	__global float* b,	
	unsigned int prime,
	unsigned int vectorSize,
	unsigned int cutPoint,
	unsigned int sampleSize,
	__global float* extraOut
)
{
	unsigned int i = get_global_id(0);

	//b[i] = 0;
	//extraOut[i] = 0;
	//return;
	
	unsigned int seed = permutateQPR(permutateQPR((i%prime), prime) + (0x682f0161 % prime), prime) % prime;
	unsigned int intermediateOffset = permutateQPR(permutateQPR((i%prime), prime)+ (0x46790905%prime) , prime);
	
	//b[i] = permutateQPR(i, prime);
	float maxFound = 0;
	float index = 0;
		
	for (int point = CPM_START_POINT; point < sampleSize - CPM_START_POINT; point = point + 1)
	{
		float val = evaluateStatistic(a, i, prime, seed, intermediateOffset, sampleSize, point, vectorSize);
		if (maxFound < val)
		{
			maxFound = val;
			index = point;
		}	
	}	
	b[i] = maxFound;
	extraOut[i] = index;
}