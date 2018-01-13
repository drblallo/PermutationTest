//4294967291

unsigned int permutateQPR(unsigned int x, unsigned int prime)
{
	unsigned int residue = (((unsigned long)x) * ((unsigned long)x)) % prime;
	
	unsigned int t = (int)(x <= prime / 2);

	unsigned int returnValue = t * residue;

	t = 1 - t;
	returnValue += (prime - residue)* t; 
	return returnValue;
}

unsigned int next(unsigned int* seed, unsigned int intermediateOffset, unsigned prime)
{
	unsigned int t = permutateQPR(((((permutateQPR(*seed, prime) + intermediateOffset) % prime)) + 0x682f0161)%prime, prime);
	*seed = (*seed + 1) % prime;
	
	return t; //to change
}

float evaluateStatistic
(
	__global const float* a,
	unsigned int index, 
	const unsigned int prime,	
	unsigned int seed,
	const unsigned int offset,
	const unsigned int sampleSize, 
	const unsigned int cutPoint,
	const unsigned int vectorSize
)
{
	unsigned int count = 0;
	float stat = 0;
	
	float firstPartAccumulation = 0;
	float secondPartAccumulation = 0;
	
	//unsigned int overBounds[prime - sampleSize];
	unsigned int overBounds[OVER_BOUND_SIZE];
	unsigned int overBoundWriteIndex = 0;
	unsigned int overBoundReadIndex = 0;
	int isFirstItem = (int)(index == 0);
	for (int a = 0; a < OVER_BOUND_SIZE; a++)
		overBounds[a] = 0;


	while (count < sampleSize)
	{
		float currentValue = a[count];		
		
		unsigned int actualPosition = next(&seed, offset, prime);	
		actualPosition = (1-isFirstItem) * (actualPosition);
		actualPosition += (isFirstItem) * count;
			
		int isAfterEdge = (int)(actualPosition >= sampleSize);
		overBounds[overBoundWriteIndex] += actualPosition * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;

		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 

		firstPartAccumulation += mustBeAddToFirstGroup * currentValue;
		secondPartAccumulation += mustBeAddToSecondGroup * currentValue;

		count = count + 1;
	}
	stat = firstPartAccumulation / (cutPoint);
	stat -= secondPartAccumulation / (sampleSize - cutPoint);
	return stat;
		
	while (count < prime)
	{
		float currentValue = overBounds[overBoundReadIndex];		
		
		unsigned int actualPosition = next(&seed, offset, prime);

		int isAfterEdge = (int)(actualPosition >= sampleSize);
		overBounds[overBoundWriteIndex] += actualPosition * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;

		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 

		firstPartAccumulation += mustBeAddToFirstGroup * currentValue;
		secondPartAccumulation += mustBeAddToSecondGroup * currentValue;

		overBoundReadIndex = overBoundReadIndex + 1;
		count = count + 1;
	}

	stat = firstPartAccumulation / (cutPoint);
	stat -= secondPartAccumulation / (sampleSize - cutPoint);
	return stat;
}

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