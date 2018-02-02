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
		overBounds[overBoundWriteIndex] += currentValue * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;

		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 

		firstPartAccumulation += mustBeAddToFirstGroup * currentValue;
		secondPartAccumulation += mustBeAddToSecondGroup * currentValue;

		count = count + 1;
	}
	
	while (count < prime)
	{
		float currentValue = overBounds[overBoundReadIndex];		
		
		unsigned int actualPosition = next(&seed, offset, prime);

		int isAfterEdge = (int)(actualPosition >= sampleSize);
		overBounds[overBoundWriteIndex] += currentValue * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;

		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 

		firstPartAccumulation += mustBeAddToFirstGroup * currentValue;
		secondPartAccumulation += mustBeAddToSecondGroup * currentValue;

		overBoundReadIndex = overBoundReadIndex + 1;
		count = count + 1;
	}
	
	
	int l0 = cutPoint;
	int l1 = sampleSize - cutPoint;
	firstPartAccumulation -= ((l0 * (1 + l0))/2);
	secondPartAccumulation -= ((l1* (1 + l1))/2);
	firstPartAccumulation = min(firstPartAccumulation, secondPartAccumulation);	
	
	//float mu = l0 * l1 / 2;
	//float sd = sqrt((l0 * l1 * (l1+l0+1)) / 12.0f);
	//firstPartAccumulation = (firstPartAccumulation-mu)/sd;

	
	return fabs(firstPartAccumulation);
}