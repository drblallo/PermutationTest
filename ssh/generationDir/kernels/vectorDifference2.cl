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
	float overBounds[OVER_BOUND_SIZE * VECTOR_SIZE];
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
		{
			overBounds[(overBoundWriteIndex * VECTOR_SIZE) + t] += currentValue.val[t] * isAfterEdge;
            //if (overBoundWriteIndex == 4)
             //   return overBounds[(overBoundWriteIndex * VECTOR_SIZE) + t];
        }
		overBoundWriteIndex += isAfterEdge;
           // if (overBoundWriteIndex == 5)
             //   return overBounds[4];

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

		//if (overBoundWriteIndex == 5)
	        //return currentValue.val[0];
	      //  return overBounds[4];
	       //return isAfterEdge;
	       // return currentValue.val[0];
	}
	//return 0;
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

		//if (count == sampleSize + 5)
	        //return currentValue.val[0];
	        //return (overBoundReadIndex-1) * VECTOR_SIZE;
	      //  return 0;
	}
	//return secondPartAccumulation.val[0];

	/*for (int t = 0; t < VECTOR_SIZE; t++)
	{

	    //THERE IS NO SUPPORT FOR DIVISION
		//firstPartAccumulation.val[t] /= cutPoint;
		//secondPartAccumulation.val[t] /= sampleSize - cutPoint;

		firstPartAccumulation.val[t] -= secondPartAccumulation.val[t];
	}*/

	float x = 0;
	float y = 0;

	for (int t = 0; t < VECTOR_SIZE; t++)
	{
		x += firstPartAccumulation.val[t] * firstPartAccumulation.val[t];
		y += secondPartAccumulation.val[t] * secondPartAccumulation.val[t];
	}
	return sqrt(x) - sqrt(y);

}
