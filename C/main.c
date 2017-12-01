#include "permutationTest.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define ALPHA 0.05f
#define ITERATIONS 1000
#define SAMPLE_SIZE 100
#define CUT_POINT 50
#define PERMUTATIONS 1000
#define THREAD_COUNT 4
#include <random>

std::default_random_engine engine;
std::normal_distribution<float> dist1(0, 1);
std::normal_distribution<float> dist2(2, 1);
std::normal_distribution<float> dist3(1, 1);
std::normal_distribution<float> dist4(2, 2);

float randomFromGaussianMixture()
{
	if (randomFloat() < 0.5f)
		return dist1(engine);
	else
		return dist2(engine);
}

float randomFromGaussianMixture2()
{
	if (randomFloat() < 0.5f)
		return dist3(engine);
	else
		return dist4(engine);
}

void generateSample(TestData* data)
{
	for (int b = 0; b < CUT_POINT; b++)
		data->sample[b] = randomFromGaussianMixture();

	for (unsigned b = CUT_POINT; b < data->sampleLenght; b++)
		data->sample[b] = randomFromGaussianMixture();
}

typedef struct thd
{
	unsigned startingIteration;
	unsigned endingIteration;
	unsigned successes;
	const TestData* data;
} ThreadData;

void* threadedSuccessRateo(void* d)
{
	ThreadData* th = (ThreadData*)d;	
	TestData data;
   	testDataClone(&data, th->data);

	for (unsigned a = th->startingIteration; a < th->endingIteration; a++)
	{
		generateSample(&data);
		th->successes += runPermutationTest(&data);
	}
	free(data.sample);
	return NULL;
}

float getPermutationSuccessRateo(int permutations, TestData* data)
{
	float count = 0;
	data->iterationsCount = permutations;
	pthread_t thread[THREAD_COUNT];
	ThreadData d[THREAD_COUNT];

	for (int a = 0; a < THREAD_COUNT; a++)
	{
		d[a].startingIteration = ITERATIONS / THREAD_COUNT * a;
		d[a].endingIteration = ITERATIONS / THREAD_COUNT * (a+1);
		d[a].data = data;
		d[a].successes = 0;
		pthread_create(&thread[a], NULL, threadedSuccessRateo,&d[a]);
	}

	for (int a = 0; a < THREAD_COUNT; a++)	
	{
		pthread_join(thread[a], NULL);
		count += d[a].successes;
	}

	return count/(float)ITERATIONS;
}

int main()
{

	//set up data
	TestData data;
	data.alpha = ALPHA;
	data.iterationsCount = PERMUTATIONS;
	data.cutPoint = CUT_POINT;
	data.sampleLenght = SAMPLE_SIZE;
	data.sample = (float*)malloc(sizeof(float)*data.sampleLenght);

	for (int a = 50; a < PERMUTATIONS; a++)
	{
		printf("%d %f\n", a, getPermutationSuccessRateo(a, &data));
		fflush(stdout);
	}

	//clean up
    free (data.sample);
}

