#pragma once
#include "permutation.h"
#define VECTOR_SIZE 4

typedef float Vector[VECTOR_SIZE];

typedef struct td
{
	float* sample;
	Vector* vectorSample;	
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
float hotelintTest(const TestData* data, const Permutation* permutation);
void testDataClone(TestData* newOne, const TestData* toCopy);
void destroyTestData(TestData* data);
