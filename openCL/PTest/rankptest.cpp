#include "rankptest.hpp"

using namespace pt;

RankPTest::RankPTest
		(
	   	unsigned  valuesCount,
	   	bool trackLocationOfChange,
	   	const char* statistic,
	   	unsigned it,
		float significativityLevel
		) 
		:	PTest
			(
				1, 
				valuesCount, 
				trackLocationOfChange, 
				statistic, 
				it, 
				significativityLevel
			),
		   	postProcessValues() {}

RankPTest::~RankPTest()
{

}


std::vector<float>& RankPTest::getVectorToLoad()
{
	return postProcessValues;
}

void RankPTest::merge(std::vector<unsigned>& indexes, int left, int right, int center)
{
	int i = left;
	int j = center;
	int k = 0;
	unsigned b[right - left + 1];

	while (i < center && j < right)
	{
		if (values[indexes[i]] <= values[indexes[j]])
		{
			b[k] = indexes[i];
			i++;
		}
		else
		{
			b[k] = indexes[j];
			j++;
		}
		k++;
	}

	while (i < center)
	{
		b[k] = indexes[i];
		i++;
		k++;
	}
	while (j < right)
	{
		b[k] = indexes[j];
		j++;
		k++;
	}

	for (k = left; k < right; k++)
		indexes[k] = b[k - left];
}

void RankPTest::calculateRank(std::vector<unsigned>& indexes, int left, int right)
{
	int center;
	if (left + 1 >= right)
		return;

	center = (left + right)/2;
	calculateRank(indexes, left, center);
	calculateRank(indexes, center, right);

	merge(indexes, left, right, center);
}

void RankPTest::sortIndexes(std::vector<unsigned>& indexes)
{
	calculateRank(indexes, 0, indexes.size());	
}

#include <iostream>
void RankPTest::preProcess()
{
	std::vector<unsigned> indexes;
	for (unsigned a = 0; a < values.size(); a++)
		indexes.push_back(a);

	sortIndexes(indexes);

	//for (int a = 0; a < indexes.size(); a++)
	//	std::cout << indexes.at(a) << std::endl;
	postProcessValues.clear();
	for (unsigned a = 0; a < values.size(); a++)
		postProcessValues.push_back(indexes[a]);
}
