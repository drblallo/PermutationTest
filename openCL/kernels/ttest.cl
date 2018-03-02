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
		actualPosition = (1-isFirstItem) * (actualPosition);
		actualPosition += (isFirstItem) * count;

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

	float mean1 = firstPartAccumulation / (cutPoint);
	float mean2 = secondPartAccumulation / (sampleSize - cutPoint);

	stat = mean1 - mean2;	
	
	return fabs(stat);
	//deviation
	count = 0;
	firstPartAccumulation = 0;
	secondPartAccumulation = 0;
	
	overBoundWriteIndex = 0;
	overBoundReadIndex = 0;
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

		firstPartAccumulation += mustBeAddToFirstGroup * ((currentValue - mean1) * (currentValue - mean1));
		secondPartAccumulation += mustBeAddToSecondGroup * ((currentValue - mean2) * (currentValue - mean2));

		count = count + 1;
	}
		
	while (count < prime)
	{
		float currentValue = overBounds[overBoundReadIndex];		
		
		unsigned int actualPosition = next(&seed, offset, prime);
		actualPosition = (1-isFirstItem) * (actualPosition);
		actualPosition += (isFirstItem) * count;

		int isAfterEdge = (int)(actualPosition >= sampleSize);
		overBounds[overBoundWriteIndex] += currentValue * isAfterEdge;
		overBoundWriteIndex += isAfterEdge;


		int mustBeAddToFirstGroup = (int)(actualPosition < cutPoint);
		int mustBeAddToSecondGroup = (int)(actualPosition >= cutPoint && !isAfterEdge); 

		firstPartAccumulation += mustBeAddToFirstGroup * ((currentValue - mean1) * (currentValue - mean1));
		secondPartAccumulation += mustBeAddToSecondGroup * ((currentValue - mean2) * (currentValue - mean2));

		overBoundReadIndex = overBoundReadIndex + 1;
		count = count + 1;
	}

	float variance1 = firstPartAccumulation / (cutPoint - 1);
	float variance2 = secondPartAccumulation / (sampleSize - cutPoint - 1);
	
	//stat /= sqrt((variance1 / cutPoint) + (variance2 / (sampleSize - cutPoint)));	
	//stat /= sqrt(variance1 + variance2);	
	//stat = stat * sqrt(float(sampleSize));

	return fabs(stat);
}
