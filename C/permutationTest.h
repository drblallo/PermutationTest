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
float evaluateStatistic(const TestData* data, const Permutation* permutation);
int runPermutationTest(const TestData* data);
float evaluateStatisticMean(const TestData* data, const Permutation* permutation);
float mannWitheyTest(const TestData* data, const Permutation* permutation);
void testDataClone(TestData* newOne, const TestData* toCopy);
