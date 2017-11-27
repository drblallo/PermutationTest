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

float evaluateStatistic(const TestData* data, const Permutation* permutation)
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

int permutationTestGetK(const TestData* data)
{
	return data->iterationsCount - ((int) (data->alpha * data->iterationsCount));
}

int runPermutationTest(const TestData* data)
{
    float statistics[data->iterationsCount];
	Permutation permutation = permutationCreate(data->sampleLenght);
	float sampleStatistic = evaluateStatistic(data, &permutation); //non permutated statistic
	
	unsigned equalOccurences = 0;
	unsigned higherOccurences = 0;
	int toBeReturned = 0;


	//calculate permutated statistics
	for (unsigned a = 0; a < data->iterationsCount; a++)
	{
		permutationRandomize(permutation);
		statistics[a] = evaluateStatistic(data, &permutation);

		if (statistics[a] == sampleStatistic)
			equalOccurences++;

		if (statistics[a] > sampleStatistic)
			higherOccurences++;
	}
	
	//sort statistics
    qsort(statistics, data->iterationsCount, sizeof (float), compare);
	int k = permutationTestGetK(data);
	
	if (statistics[k] > sampleStatistic)
		toBeReturned = 1;

	if (statistics[k] < sampleStatistic)
		toBeReturned = 0;

	if (statistics[k] == sampleStatistic)
	{
		float treshold = (data->alpha * data->iterationsCount) - higherOccurences;
		treshold /= equalOccurences;

		if (randomFloat(0, 1) > treshold)
			toBeReturned = 0;
		else
			toBeReturned = 1;
	}

	permutationDestory(permutation);
	return toBeReturned;
}
