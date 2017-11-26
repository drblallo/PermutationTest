#pragma once
#include "permutation.h"

typedef struct td
{
	float* sample;
	unsigned sampleLenght;
	unsigned cutPoint;
	unsigned iterationsCount;
	float alpha;		
} TestData;

typedef struct prm Permutation;

int runPermutationTest(const TestData* data);
float evaluateStatistic(const TestData* data);
float evaluateStatisticWithPermutation(const TestData* data, const Permutation* permutation);
