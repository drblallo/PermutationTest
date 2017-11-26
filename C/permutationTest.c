#include "permutationTest.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

int compare(const void * a, const void * b)
{
	float v1 = *((float*)a);
	float v2 = *((float*)b);

    if (v1 < v2) return -1;
	if (v1 == v2) return 0;
	return 1; 
}

float evaluateStatistic(const TestData* data)
{
	float v1 = 0;
	float v2 = 0;
	for (unsigned a = 0; a < data->cutPoint; a++)
		v1 += data->sample[a];

	v1 /= data->cutPoint;

	for (unsigned a = data->cutPoint; a < data->sampleLenght; a++)
		v2 += data->sample[a];

	v2 /= data->sampleLenght - data->cutPoint;

   // printf("%f %f\n", v1, v2);

	v1 -= v2;

	if (v1 < 0)
		v1 *= -1;

	return v1;
}

float evaluateStatisticWithPermutation(const TestData* data, const Permutation* permutation)
{
	float v1 = 0;
	float v2 = 0;
	for (unsigned a = 0; a < data->cutPoint; a++)
		v1 += data->sample[permutation->indicies[a]];

	v1 /= data->cutPoint;

	for (unsigned a = data->cutPoint; a < data->sampleLenght; a++)
		v2 += data->sample[permutation->indicies[a]];

	v2 /= data->sampleLenght - data->cutPoint;

	v1 -= v2;

	if (v1 < 0)
		v1 *= -1;

	return v1;
}

int runPermutationTest(const TestData* data)
{
    float statistics[data->iterationsCount];
	float sampleStatistic = evaluateStatistic(data);
    //printf("sample: %f\n", sampleStatistic);
	
	unsigned equalOccurences = 0;
	unsigned higherOccurences = 0;

	Permutation permutation = permutationCreate(data->sampleLenght);

	for (unsigned a = 0; a < data->iterationsCount; a++)
	{
		permutationRandomize(permutation);
		//for (int a = 0; a < data->sampleLenght; a++)
		//	printf("%d\n", permutation.indicies[a]);
		statistics[a] = evaluateStatisticWithPermutation(data, &permutation);

		if (statistics[a] == sampleStatistic)
			equalOccurences++;

		if (statistics[a] > sampleStatistic)
			higherOccurences++;
	}
	
    qsort(statistics, data->iterationsCount, sizeof (float), compare);
	int k = data->iterationsCount - ((int)(data->alpha * data->iterationsCount));
    //for (int a = 0; a < data->iterationsCount; a++)
    //    printf("%f\n", statistics[a]);

    //printf ("k: %f\n", statistics[k]);
	
	if (statistics[k] > sampleStatistic)
	{
		permutationDestory(permutation);
		return 1;
	}

	if (statistics[k] < sampleStatistic)
	{
		permutationDestory(permutation);
		return 0;
	}

	float treshold = (data->alpha*data->iterationsCount) - higherOccurences;
	treshold /= equalOccurences;

	if (randomFloat(0, 1) > treshold)
	{
		permutationDestory(permutation);
		return 0;
	}

	permutationDestory(permutation);
	return 1;
}
