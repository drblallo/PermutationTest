#include "permutationTest.h"
#include "utils.h"
#include <math.h>
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
	float v = data->statistic(data, permutation);
	return v;
}

float evaluateStatisticMean(const TestData* data, const Permutation* permutation)
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

float hotelintTest(const TestData* data, const Permutation* toCopy)
{
	Vector mean;
	Vector mean2;
	for (unsigned b = 0; b < VECTOR_SIZE; b++)
	{
		mean[b] = 0;
		for (unsigned a = 0; a < data->cutPoint; a++)
			mean[b] += data->vectorSample[toCopy->indicies[a]][b]; 
		mean[b] /= data->sampleLenght;

		mean2[b] = 0;
		for (unsigned a = data->cutPoint; a < data->sampleLenght; a++)
			mean2[b] += data->vectorSample[toCopy->indicies[a]][b]; 
		mean2[b] /= data->sampleLenght;
	}

	float** matrix = (float**) malloc(VECTOR_SIZE * sizeof(float*));
	float** matrix2 = (float**) malloc(VECTOR_SIZE * sizeof(float*));

	for (unsigned a = 0; a < VECTOR_SIZE; a++)
	{
		matrix[a] = (float*) malloc(VECTOR_SIZE* sizeof(float));
		matrix2[a] = (float*) malloc(VECTOR_SIZE* sizeof(float));
	}
	for (unsigned a = 0; a < VECTOR_SIZE; a++)
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
		{
			matrix[a][b] = 0;
			matrix2[a][b] = 0;
		}

	for (unsigned a = 0; a < data->cutPoint; a++)
	{
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
		{
			for (unsigned c = 0; c < VECTOR_SIZE; c++)
			{
				float t = (data->vectorSample[toCopy->indicies[a]][b] - mean[b]);
				float r = (data->vectorSample[toCopy->indicies[a]][c] - mean[c]);
				matrix[b][c] += t * r; 
			}
		}
	}

	for (unsigned a = data->cutPoint; a < data->sampleLenght; a++)
	{
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
		{
			for (unsigned c = 0; c < VECTOR_SIZE; c++)
			{
				float t = (data->vectorSample[toCopy->indicies[a]][b] - mean2[b]);
				float r = (data->vectorSample[toCopy->indicies[a]][c] - mean2[c]);
				matrix2[b][c] += t * r; 
			}
		}
	}

	for (unsigned a = 0; a < VECTOR_SIZE; a++)
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
			matrix[a][b] = (matrix[a][b] + matrix2[a][b]) / (data->sampleLenght + 2);

	float determinante = Determinant(matrix, VECTOR_SIZE);


	for (unsigned a = 0; a < VECTOR_SIZE; a++)
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
			matrix[a][b] /= determinante;

	Vector partial;

	for (unsigned a = 0; a < VECTOR_SIZE; a++)
	{
		partial[a] = 0;
		for (unsigned b = 0; b < VECTOR_SIZE; b++)
			partial[a] += matrix[a][b] * (mean[b] - mean2[b]);
	}

	float out = 0;
	for (unsigned a = 0; a < VECTOR_SIZE; a++)
		out += (mean[a] - mean2[a]) * partial[a];

	for (int a = 0; a < VECTOR_SIZE; a++)
	{
		free(matrix[a]);
		free(matrix2[a]);
	}
	free (matrix);
	free (matrix2);


	return out;

}

void merge(float** ranksOut, int left, int center, int right)
{
	int i = left;
	int j = center;
	int k = 0;
	float* b[right-left+1];

	while (i < center && j < right)
	{
		//printf ("%d %d\n", );
		if (*(ranksOut[i]) <= *(ranksOut[j]))
		{
			b[k] = ranksOut[i];	
			i++;
		}
		else
		{
			b[k] = ranksOut[j];
			j++;
		}
		k++;
	}

	while (i < center)
	{
		b[k] = ranksOut[i];
		i++;
		k++;
	}

	while (j < right)
	{
		b[k] = ranksOut[j];
		j++;
		k++;
	}

	for (k = left; k < right; k++)
		ranksOut[k] = b[k-left];
}

void calculateRanks(float** ranksOut, int left, int right)
{
	int center;
	if (left + 1>= right)
		return;

	center = (left+right)/2;
	calculateRanks(ranksOut, left, center);
	calculateRanks(ranksOut, center, right);

	merge(ranksOut, left, center, right);
}

float mannWitheyTest(const TestData* data, const Permutation* permutation)
{
	float** ranks = (float**)malloc(data->sampleLenght * sizeof(float*));	
	float* values = (float*)malloc(data->sampleLenght * sizeof(float));	
	float l1 = data->sampleLenght - data->cutPoint;
	float l0 = data->cutPoint;
	
	
	for (unsigned a = 0; a < data->sampleLenght; a++)
	{
		values[a] = data->sample[permutation->indicies[a]];
		ranks[a] = values + a;
	}
	calculateRanks(ranks, 0, data->sampleLenght);

	float u1 = 0;
	float u2 = 0;

	for (unsigned a = 0; a < data->sampleLenght; a++)
	{
		//printf("%d %f\n", ranks[a]-data->sample, *ranks[a]);
		if (ranks[a] - values < l0)
			u1 += a;
		else
			u2 += a;
	}

	u1 -= (l0*(l0+1))/2;

	//u2 -= ((data->sampleLenght-data->cutPoint)*((data->sampleLenght-data->cutPoint)+1))/2;
	//printf("%f %f\n", u1, u2);

	free(ranks);
	free(values);
	//if (u2 > u1)
	//	return u1;
	//else
	//	return u2;
	
	float mu = l0 * l1;
	float sd = sqrt(l0 * l1 * (l1+l0+1) / 12.0f); 
	u1 = fabs((u1-mu)/sd);
	//printf("%f\n", u1);
	return u1;
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

		if (randomFloat() > treshold)
			toBeReturned = 0;
		else
			toBeReturned = 1;
	}

	permutationDestory(permutation);
	return toBeReturned;
}

void testDataClone(TestData* newOne, const TestData* toCopy)
{
	newOne->alpha = toCopy->alpha;
	newOne->sampleLenght = toCopy->sampleLenght;
	newOne->cutPoint = toCopy->cutPoint;
	newOne->iterationsCount = toCopy->iterationsCount;
	newOne->statistic = toCopy->statistic;
	if (toCopy->sample)
	{
		newOne->sample = (float*) malloc(sizeof(float) * toCopy->sampleLenght);
		for (unsigned a = 0; a < toCopy->sampleLenght; a++)
			newOne->sample[a] = toCopy->sample[a];
	}
	else
		newOne->sample = NULL;

	if (toCopy->vectorSample)
	{
		newOne->vectorSample = (Vector*) malloc(sizeof(Vector) *toCopy->sampleLenght);
		for (unsigned a = 0; a < toCopy->sampleLenght; a++)
			for (unsigned b = 0; b < VECTOR_SIZE; b++)
			newOne->vectorSample[a][b] = toCopy->vectorSample[a][b];
	}
	else
		newOne->vectorSample = NULL;

}

void destroyTestData(TestData* data)
{
	if (data->sample)
		free(data->sample);
	if (data->vectorSample)
		free(data->vectorSample);
}
