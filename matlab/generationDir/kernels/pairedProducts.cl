float evaluateStatistic
(
	__global const float* in,
	unsigned int index,
	const unsigned int prime,
	unsigned int sd,
	const unsigned int offset,
	const unsigned int sampleSize,
	const unsigned int cutPoint,
	const unsigned int vectorSize
)
{
	float s = 0;

	for (int a = 0; a < cutPoint; a++)
	{
		for (int b = 0; b < VECTOR_SIZE; b++)
			s += in[(a * VECTOR_SIZE) + b] * in[cutPoint + ((a * VECTOR_SIZE) + b)];		
	}

	s = s / cutPoint;
	return s;
}
