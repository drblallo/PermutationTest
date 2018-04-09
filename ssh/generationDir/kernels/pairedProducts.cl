unsigned int rand(unsigned int* seed, int sampleSize) // 1 <= *seed < m
{
    unsigned int const a = 16807; //ie 7**5

    *seed = (*seed * a) % sampleSize;
    return(*seed);
}

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
	unsigned int seed = sd;

	for (int a = 0; a < cutPoint; a++)
	{
	    unsigned int v1 = rand(&seed, sampleSize);
	    unsigned int v2 = rand(&seed, sampleSize);
	    v1 = (v1 * ((int) index != 0)) + (a * ((int) index == 0));
	    v2 = (v2 * ((int) index != 0)) + (a * ((int) index == 0));
		for (int b = 0; b < VECTOR_SIZE; b++)
			s += in[(v1 * VECTOR_SIZE) + b] * in[cutPoint + ((v2 * VECTOR_SIZE) + b)];
	}

	s = s / cutPoint;
	return s;
}
