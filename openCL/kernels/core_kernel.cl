__kernel void p_test
(
	__global const float* a,
	__global float* b,
	unsigned int prime,
	unsigned int vectorSize,
	unsigned int cutPoint,
	unsigned int sampleSize
)
{
	unsigned int i = get_global_id(0);
	unsigned int seed = permutateQPR(permutateQPR((i%prime), prime) + (0x682f0161 % prime), prime) % prime;
	unsigned int intermediateOffset = permutateQPR(permutateQPR((i%prime), prime)+ (0x46790905%prime) , prime);
	
	//b[i] = permutateQPR(i, prime);
	b[i] = evaluateStatistic(a, i, prime, seed, intermediateOffset, sampleSize, cutPoint, vectorSize);
}