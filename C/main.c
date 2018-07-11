#include "permutationTest.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#define ALPHA 0.05f
#define ITERATIONS 10000
#define SAMPLE_SIZE 50
#define CUT_POINT 25
#define PERMUTATIONS 10000
#define THREAD_COUNT 4


void generateSample(TestData* data, float (*fun1)(), float(*fun2)())
{
	for (int b = 0; b < CUT_POINT; b++)
		data->sample[b] = fun1();

	for (unsigned b = CUT_POINT; b < data->sampleLenght; b++)
		data->sample[b] = fun2();
}

void generateSampleVector(TestData* data, float (*fun1)(), float (*fun2)())
{
	for (unsigned a = 0; a < data->cutPoint; a++)
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
			data->vectorSample[a][b] = fun1();
	for (unsigned a = data->cutPoint; a < data->sampleLenght; a++)
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
			data->vectorSample[a][b] = fun2();
}

typedef struct thd
{
	unsigned startingIteration;
	unsigned endingIteration;
	unsigned successes;
	const TestData* data;
	float (*fun1)();
	float (*fun2)();
	FILE* file;
} ThreadData;


pthread_mutex_t mutex;

void fAtomicPrint(int iteration, int permutations, int success, FILE* f)
{
	pthread_mutex_lock(&mutex);
	fprintf(f, "%d %d %d\n", iteration, permutations, success);	
	pthread_mutex_unlock(&mutex);
}

void runSingleTest(TestData* data, int iteration, FILE* f)
{
	for (int a = 1; a < PERMUTATIONS; a = a * 2)
	{
		int c = 0;
		data->iterationsCount = a;
		c = runPermutationTest(data);
		fAtomicPrint(iteration, a, c, f);
	}	
}

void* threadedSuccessRateo(void* d)
{
	ThreadData* th = (ThreadData*)d;	
	TestData data;
   	testDataClone(&data, th->data);

	for (unsigned a = th->startingIteration; a < th->endingIteration; a++)
	{
		printf("%d\n", a);
		generateSample(&data, th->fun1, th->fun2);
		runSingleTest(&data, a, th->file);
		//th->successes += runPermutationTest(&data);
	}
	destroyTestData(&data);
	return NULL;
}

float runTests(TestData* data, float (*fun1)(), float (*fun2)(), FILE* f)
{
	float count = 0;
	data->iterationsCount = PERMUTATIONS;
	pthread_t thread[THREAD_COUNT];
	ThreadData d[THREAD_COUNT];

	for (int a = 0; a < THREAD_COUNT; a++)
	{
		d[a].startingIteration = ITERATIONS / THREAD_COUNT * a;
		d[a].endingIteration = ITERATIONS / THREAD_COUNT * (a+1);
		d[a].data = data;
		d[a].successes = 0;
		d[a].fun1 = fun1;
		d[a].fun2 = fun2;
		d[a].file = f;
		pthread_create(&thread[a], NULL, threadedSuccessRateo,&d[a]);
	}

	for (int a = 0; a < THREAD_COUNT; a++)	
	{
		pthread_join(thread[a], NULL);
		count += d[a].successes;
	}

	return count/(float)ITERATIONS;
}

#include<sys/timeb.h>

#include <stdint.h>
uint64_t now()
{
	struct timeb timerBuffer;
	ftime(&timerBuffer);
	return (uint64_t) (((timerBuffer.time * 1000)+ timerBuffer.time));
}

void testSpeed()
{
	for (int a = 2; a < 10000; a = a * 2)
	{
		TestData data;
		data.alpha = ALPHA;
		data.iterationsCount = a;
		data.sampleLenght = 1000;
		data.cutPoint = 500;
		data.sample = (float*) malloc(sizeof(float) * data.sampleLenght);
		data.statistic = evaluateStatisticMean;
		
		for (int b = 0; b < 1000; b++)
			data.sample[b] = 0;
	

		uint64_t before = now();
		for (int b = 0; b < 1000; b++)
			runPermutationTest(&data);
		uint64_t after = now();

		printf("permutation %d: elapsed %lu milliseconds\n", a, after - before);
	
	}
}

int main()
{

	testSpeed();
	return 0;

	FILE* f;

	//set up data
	TestData data;
	data.alpha = ALPHA;
	data.iterationsCount = PERMUTATIONS;
	data.cutPoint = CUT_POINT;
	data.sampleLenght = SAMPLE_SIZE;
	data.sample = (float*) malloc(sizeof(float) * data.sampleLenght);
	data.vectorSample= NULL;
	data.statistic = mannWitheyTest;
	//data.vectorSample = (Vector*) malloc(sizeof(Vector) * data.sampleLenght);
	pthread_mutex_init(&mutex, NULL);

	printf("uniform equal\n");
	f = fopen("uniform_equal.txt", "w+");
	runTests(&data, randomFromUniform, randomFromUniform, f);
	fclose(f);

	printf("uniform disequal\n");
	f = fopen("uniform_0,1.txt", "w+");
	runTests(&data, randomFromUniform, randomFromUniform2, f);
	fclose(f);

	printf("normal equal\n");
	f = fopen("normal_equal.txt", "w+");
	runTests(&data, randomFromNormal, randomFromNormal, f);
	fclose(f);

	printf("normal disequal\n");
	f = fopen("normal_0,5.txt", "w+");
	runTests(&data, randomFromNormal, randomFromNormal2, f);
	fclose(f);

	printf("chi squared equal\n");
	f = fopen("chi_squared_equal.txt", "w+");
	runTests(&data, randomFromChiSquared1, randomFromChiSquared1, f);
	fclose(f);

	printf("chi squared disequal\n");
	f = fopen("chi_squared_1.txt", "w+");
	runTests(&data, randomFromChiSquared1, randomFromChiSquared2, f);
	fclose(f);

	printf("mixture equal\n");
	f = fopen("mixed_equal.txt", "w+");
	runTests(&data, randomFromGaussianMixture1, randomFromGaussianMixture1, f);
	fclose(f);

	printf("mixture disequal\n");
	f = fopen("mixed_disequal.txt", "w+");
	runTests(&data, randomFromGaussianMixture1, randomFromGaussianMixture2, f);
	fclose(f);

	//clean up
	destroyTestData(&data);
}

