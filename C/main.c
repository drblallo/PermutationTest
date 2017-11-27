#include "permutationTest.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#define ALPHA 0.05f
#define ITERATIONS 1000
#define SAMPLE_SIZE 100
#define CUT_POINT 50
#define PERMUTATIONS 1000

void generateSample(TestData* data)
{
		for (int b = 0; b < CUT_POINT; b++)
            data->sample[b] = randomFloat();

		for (unsigned b = CUT_POINT; b < data->sampleLenght; b++)
            data->sample[b] = randomFloat();
}

int main()
{
	int count = 0;

	//set up data
	TestData data;
	data.alpha = ALPHA;
	data.iterationsCount = PERMUTATIONS;
	data.cutPoint = CUT_POINT;
	data.sampleLenght = SAMPLE_SIZE;
	data.sample = (float*)malloc(sizeof(float)*data.sampleLenght);

	//run tests
	for (int a = 0; a < ITERATIONS; a++)
	{
		generateSample(&data);
		count += runPermutationTest(&data);
	}

	//report tests
	printf("%d out of %d\n", count, ITERATIONS);

	//clean up
    free (data.sample);
}
