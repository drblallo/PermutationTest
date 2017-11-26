#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include <math.h>

int initRandom = 0;
int randomNumber(int min, int max)
{
	if (initRandom == 0)
	{
		initRandom = 1;
		srand(time(NULL));
	}
	
	int r;

	const unsigned int range = 1 + max - min;
	const unsigned int buckets = RAND_MAX / range;
	const unsigned int limit = buckets * range;

	do
	{
		r = rand();
	} while (r >= limit);

	return min + (r/buckets);
}

float randomFloat()
{
	if (initRandom == 0)
	{
		initRandom = 1;
		srand(time(NULL));
	}

    return (float)rand()/(float)RAND_MAX;
}
