typedef struct vct
{
	float val[VECTOR_SIZE];
} Vector;

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

	float stat = 0;
	unsigned int count = 0;

	
	Vector firstPartAccumulation;
	Vector secondPartAccumulation;
	
	//unsigned int overBounds[prime - sampleSize];
	unsigned int overBounds[OVER_BOUND_SIZE * VECTOR_SIZE];
	unsigned int overBoundWriteIndex = 0;
	unsigned int overBoundReadIndex = 0;
	int isFirstItem = (int)(index == 0);
	for (int t = 0; t < OVER_BOUND_SIZE * VECTOR_SIZE; t++)
		overBounds[t] = 0;

	for (int t = 0; t < VECTOR_SIZE; t++)
	{
		firstPartAccumulation.val[t] = 0;
		secondPartAccumulation.val[t] = 0;
	}

	while (count < sampleSize)
	{
		Vector currentValue;
		for (int t = 0; t < VECTOR_SIZE; t++)		
			currentValue.val[t] = a[(count * VECTOR_SIZE) + t];		
		
		unsigned int actualPosition = next(&seed, offset, prime);	
		actualPosition = (1-isFirstItem) * (actualPosition);
		actualPosition += (isFirstItem) * count;
			
		int isAfterEdge = (int)(actualPosition >= sampleSize);
		for (int t = 0; t < VECTOR_SIZE; t++)
			overBounds[(overBoundWriteIndex * VECTOR_SIZE) + t] += currentValue.val[t] * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;

		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 
			
		for (int t = 0; t < VECTOR_SIZE; t++)
		{
			firstPartAccumulation.val[t] += mustBeAddToFirstGroup * currentValue.val[t];
			secondPartAccumulation.val[t] += mustBeAddToSecondGroup * currentValue.val[t];
		}
		//if (count == 270)
		//	return 0;
		count = count + 1;
	}
	while (count < prime)
	{
		Vector currentValue;
		for (int t = 0; t < VECTOR_SIZE; t++)
			currentValue.val[t] = overBounds[(overBoundReadIndex * VECTOR_SIZE) + t];		
		
		unsigned int actualPosition = next(&seed, offset, prime);
		actualPosition = (1-isFirstItem) * (actualPosition);
		actualPosition += (isFirstItem) * count;

		int isAfterEdge = (int)(actualPosition >= sampleSize);
		for (int t = 0; t < VECTOR_SIZE; t++)
			overBounds[(overBoundWriteIndex * VECTOR_SIZE) + t] += currentValue.val[t] * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;


		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 

		
		for (int t = 0; t < VECTOR_SIZE; t++)
		{
			firstPartAccumulation.val[t] += mustBeAddToFirstGroup * currentValue.val[t];
			secondPartAccumulation.val[t] += mustBeAddToSecondGroup * currentValue.val[t];
		}

		overBoundReadIndex = overBoundReadIndex + 1;
		count = count + 1;
	}

	for (int t = 0; t < VECTOR_SIZE; t++)
	{
		firstPartAccumulation.val[t] /= sampleSize;
		secondPartAccumulation.val[t] /= sampleSize;	
	}
	
	float mean1 = 0;
	float mean2 = 0;
	for (int t = 0; t < VECTOR_SIZE; t++)
	{
		mean1 += firstPartAccumulation.val[t] * firstPartAccumulation.val[t];
		mean2 += secondPartAccumulation.val[t] * secondPartAccumulation.val[t];	
	}	
	mean1 = sqrt(mean1);
	mean2 = sqrt(mean2);

	stat = mean1 - mean2;	
	
	return fabs(stat);
	
}
