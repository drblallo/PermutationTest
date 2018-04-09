
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
	float isFirstItem = (int)(index == 0);
	return 1-isFirstItem;
;
}
