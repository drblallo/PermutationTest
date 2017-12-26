constant unsigned int prime = 1049;//4294967291;

__kernel void p_test(__global const float* a, __global float* b)
{
	unsigned int i = get_global_id(0);
	unsigned int seed = permutateQPR(permutateQPR(i) + Ox682f0161);
	unsigned int intermediateOffset = permutateQPR(permutateQPR(i) + 0x46790905);
	
	b[i] = evaluateStatistic(a, i, seed, intemediateOffset, 1000, 500);
}

__kernel unsigned int next(unsigned int* seed, unsigned int intermediateOffset)
{
	unsigned int t = permutateQPR((permutateQPR(seed) + intermediateOffset) ^ 0x5bf03635);
	*seed = *seed + 1;
	return t;
}

__kernel void permutateQPR(unsigned int x)
{
	unsigned int residue = (((unsigned long)x) * ((unsigned long)x)) % prime;
	
	int t = (int)(x <= prime / 2);
	return (t * residue) + ((1 - t)(prime - residue)); 
}

__kernel float evaluateStatistic(__global const float* a, unsigned int index, unsigned int seed, unsigned int offset, unsigned int size, unsigned int cutPoint)
{
	unsigned int count = 0;
	float stat = 0;
	
	unsigned 

	while (count < prime)
	{
		float currentValue = a[count];		

		count = count + 1;
	}
}