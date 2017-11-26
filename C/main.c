#include "permutationTest.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#define ALPHA 0.05f
#define ITERATIONS 1000
#define SAMPLE_SIZE 100
#define CUT_POINT 50
#define PERMUTATIONS 1000

int main()
{
	int count = 0;
	TestData data;
	data.alpha = ALPHA;
	data.iterationsCount = PERMUTATIONS;
	data.cutPoint = CUT_POINT;
	data.sampleLenght = SAMPLE_SIZE;
	data.sample = (float*)malloc(sizeof(float)*data.sampleLenght);
	for (int a = 0; a < ITERATIONS; a++)
	{
		for (int b = 0; b < CUT_POINT; b++)
		{
            data.sample[b] = randomFloat();//distribution(generator);
           // printf("%f\n", data.sample[b]);
		}
		for (unsigned b = CUT_POINT; b < data.sampleLenght; b++)
		{
            data.sample[b] = randomFloat();//distribution(generator);
            //printf("%f\n", data.sample[b]);
        }

		count += runPermutationTest(&data);
	}

	printf("%d out of %d\n", count, ITERATIONS);

    free (data.sample);
}
