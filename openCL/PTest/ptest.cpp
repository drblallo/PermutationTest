#include "ptest.hpp"
#include "kernelloader.hpp"
#include "CL/clext.h"

using namespace pt;


PTest::PTest(unsigned vectorS, unsigned  valuesCount, bool trackLocationOfChange, const char* statistic) :
	values(valuesCount),
	mustCreateProgram(true),
	mustLoadData(true),
	vectorSize(vectorS),
	changeFound(false),
	changeLocation(0),
	testExecuted(false),
	useCPM(trackLocationOfChange),
	statisticName(statistic),
	buffers(0),
	programs(0),
	kernels(0)
{
	assert(KernelLoader::areCompatible(statistic, vectorS) == true);
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
	statisticName(other.statisticName),
	buffers(0),
	programs(0),
	kernels(0)
{
	
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
	kernels.clear();
	buffers.clear();
	mustCreateProgram = true;
	mustLoadData = true;
	return *this;
}

void PTest::runTest()
{
	if (mustCreateProgram)	
		createProgram();
	if (mustLoadData)
		loadData();
	
	outValues.resize(values.size());
	auto queues(Environement::getQueues());
	for (unsigned a = 0; a < queues->size(); a++)
	{
		unsigned workSize(values.size() / queues->size());
		cl_int error = queues->at(a).enqueueNDRangeKernel(
				kernels[a],
			   	cl::NDRange(workSize * a),
			   	cl::NDRange((workSize * (a + 1)) - (workSize * a)),
			   	cl::NullRange);	

		clCheckError(error);
	}

	for (unsigned a = 0; a < queues->size(); a++)
	{
		queues->at(a).finish();
	}

	queues->at(0).enqueueReadBuffer(buffers[1], CL_TRUE, 0, sizeof(float) * values.size(), &outValues[0]);
}

void PTest::createProgram()
{
	assert(programs.size() == 0);
	assert(buffers.size() == 0);

	cl::Program::Sources source;
	std::string s(KernelLoader::getProgramm(statisticName.c_str(), useCPM, vectorSize));
	source.push_back(s.c_str(), s.size());

	for (unsigned a = 0; a < Environement::getContextes()->size(); a++)
	{
		programs.push_back(cl::Program(Environement::getContextes()->at(a), source));
		int err = programs[a].build(*Environement::getDevices());
		clCheckError(err);
	}

	for (unsigned a = 0; a < Environement::getQueues()->size(); a++)
		kernels.push_back(cl::Kernel(programs[0], "p_test"));	
	
	mustCreateProgram = false;	
}

void PTest::loadData()
{
	buffers.clear();
	buffers.push_back(cl::Buffer(Environement::getContextes()->at(0), CL_MEM_READ_WRITE, sizeof(float) * values.size()));
	buffers.push_back(cl::Buffer(Environement::getContextes()->at(0), CL_MEM_READ_WRITE, sizeof(float) * values.size()));

	Environement::getQueues()->at(0).enqueueWriteBuffer(
			buffers[0],
			CL_TRUE,
			0,
			sizeof(float) * values.size(),
			&values.at(0));

	for (unsigned a = 0; a < kernels.size(); a++)
	{
		kernels[a].setArg(0, buffers[0]);
		kernels[a].setArg(0, buffers[1]);
	}

	mustLoadData = false;
}

PTest::~PTest()
{
	kernels.clear();
	programs.clear();
	buffers.clear();
}
