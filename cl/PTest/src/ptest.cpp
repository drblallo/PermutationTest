#include "ptest.hpp"
#include "kernelloader.hpp"
#include "clext.h"
#include <algorithm>
#include <random>

using namespace pt;


PTest::PTest
		(
		unsigned vectorS,
	   	unsigned  valuesCount,
	   	const char* statistic,
	   	unsigned it,
		float significativityLevel,
	   	bool trackLocationOfChange 
		) :
	values(valuesCount * vectorS),
	mustCreateProgram(true),
	mustLoadData(true),
	vectorSize(vectorS),
	changeFound(false),
	changeLocation(0),
	testExecuted(false),
	useCPM(trackLocationOfChange),
	iterations(it),
	cutPoint(0),
	alpha(significativityLevel),
	statisticName(statistic),
	gpuData()
{
	assert(KernelLoader::areCompatible(statistic, vectorS) == true);
	assert(cutPoint < values.size() == true);
	setUpGPUData();	
}

//when a test is copied from another one only cpu data are kept,
//everything helded on the gpu side must be reallocated and recalculated
PTest::PTest(const PTest& other) :
	values(other.values),
	mustCreateProgram(true),
	mustLoadData(true),
	vectorSize(other.vectorSize),
	changeFound(false),
	changeLocation(0),
	testExecuted(false),
	useCPM(other.useCPM),
	iterations(other.iterations),
	cutPoint(other.cutPoint),
	alpha(other.alpha),
	statisticName(other.statisticName),
	gpuData()
{
	setUpGPUData();	
}

PTest& PTest::operator=(const PTest& other)
{
	values = other.values;
	vectorSize = other.vectorSize;
	changeFound = false;
	changeLocation = 0;
	testExecuted = false;
	useCPM = other.useCPM;
	statisticName = other.statisticName;
	iterations = other.iterations;
	mustCreateProgram = true;
	mustLoadData = true;
	cutPoint = other.cutPoint;
	alpha = other.alpha;
	gpuData.clear();
	setUpGPUData();	
	return *this;
}

void PTest::setUpGPUData()
{
	assert(Environement::initExecuted() == true);
	gpuData.clear();
	for (unsigned a = 0; a < Environement::getContextSize(); a++)
		gpuData.push_back(PTestGPUData(&Environement::getContextDevQue()->at(a)));
	prime = (KernelLoader::getNearestPrime(values.size() / vectorSize));
}

#include <iostream>
void PTest::finilizeTest()
{
	float evaluatedStatistic(outValues[0]);
	unsigned k = iterations - (alpha * iterations);

	std::sort(outValues.begin(), outValues.end(), std::less<float>());

	if (outValues[k] < evaluatedStatistic)
	{
		changeFound = true;
		return;
	}

	if (outValues[k] > evaluatedStatistic)
	{
		changeFound = false;
		return;
	}

	unsigned higherOccurences(0);
	unsigned equalOccurences(0);
	for (unsigned a = 0; a < outValues.size(); a++)
	{
		if (evaluatedStatistic == outValues[a])
			equalOccurences++;
		if (evaluatedStatistic < outValues[a])
			higherOccurences++;
	}

	float treshold((alpha * iterations) - higherOccurences);
	treshold /= equalOccurences;
		
	std::default_random_engine eng;
	std::uniform_real_distribution<float> dist(0, 1);

	if (dist(eng) > treshold)
		changeFound = true;
	else
		changeFound = false;
}

void PTest::runTest()
{
	if (mustCreateProgram)
		createProgram();

	if (mustLoadData)
		loadData();

	outValues.resize(iterations);
	extraOutValues.resize(iterations);
	unsigned qSize(Environement::getQueuesSize());
	unsigned workSize(iterations / qSize);
	unsigned offset(0);
	for (unsigned a = 0; a < gpuData.size(); a++)
	{
		gpuData[a].run(&outValues[offset * workSize], &extraOutValues[offset * workSize]);
		offset += gpuData[a].deviceCount(); 
	}

	for (unsigned a = 0; a < gpuData.size(); a++)
		gpuData[a].waitForEnd();

	//assert(false);
	finilizeTest();
	if (useCPM && changeFound)
		changeLocation = extraOutValues[0];

    testExecuted = true;
}

void PTest::createProgram()
{
	cl::Program::Sources source;
	std::string s
		(
			KernelLoader::getProgram
			(
				statisticName.c_str(),
			   	useCPM,
			   	vectorSize,
				values.size() / vectorSize,
				prime
		   	)
		);
	source.push_back({s.c_str(), s.size()});

	for (unsigned a = 0; a < gpuData.size(); a++)
		gpuData[a].createProgram(source);

	mustCreateProgram = false;	
}

std::vector<float>& PTest::getVectorToLoad()
{
	return values;
}

void PTest::preProcess()
{

}

void PTest::loadData()
{
	unsigned qSize(Environement::getQueuesSize());
	unsigned workSize(iterations / qSize);

	preProcess();
	for (unsigned a = 0; a < gpuData.size(); a++)
		gpuData[a].loadData
			(
				getVectorToLoad().size(),
			   	&getVectorToLoad()[0],
			   	workSize,
				prime,
			   	vectorSize,
				cutPoint
			);

	mustLoadData = false;
}

PTest::~PTest()
{
	gpuData.clear();
}
