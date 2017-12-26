#include "ptest.hpp"
#include "kernelloader.hpp"
#include <CL/clext.h>

using namespace pt;


PTest::PTest(unsigned vectorS, unsigned  valuesCount, bool trackLocationOfChange, const char* statistic, unsigned it) :
	values(valuesCount),
	mustCreateProgram(true),
	mustLoadData(true),
	vectorSize(vectorS),
	changeFound(false),
	changeLocation(0),
	testExecuted(false),
	useCPM(trackLocationOfChange),
	iterations(it),
	statisticName(statistic),
	gpuData()
{
	assert(KernelLoader::areCompatible(statistic, vectorS) == true);
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

}

void PTest::runTest()
{
	if (mustCreateProgram)	
		createProgram();
	if (mustLoadData)
		loadData();
	
	outValues.resize(iterations);
	unsigned qSize(Environement::getQueuesSize());
	unsigned workSize(iterations / qSize);

	unsigned offset(0);
	for (unsigned a = 0; a < gpuData.size(); a++)
	{
		gpuData[a].run(&outValues[offset * workSize]);	
		offset += gpuData[a].deviceCount(); 
	}

	for (unsigned a = 0; a < gpuData.size(); a++)
		gpuData[a].waitForEnd();

	changeLocation = outValues[0];
    testExecuted = true;
}

void PTest::createProgram()
{
	cl::Program::Sources source;
	std::string s(KernelLoader::getProgram(statisticName.c_str(), useCPM, vectorSize));
	source.push_back({s.c_str(), s.size()});

	for (unsigned a = 0; a < gpuData.size(); a++)
		gpuData[a].createProgram(source);

	mustCreateProgram = false;	
}

void PTest::loadData()
{
	unsigned qSize(Environement::getQueuesSize());
	unsigned workSize(iterations / qSize);

	for (unsigned a = 0; a < gpuData.size(); a++)
		gpuData[a].loadData(values.size(), &values[0], workSize);

	mustLoadData = false;
}

PTest::~PTest()
{
	gpuData.clear();
}
