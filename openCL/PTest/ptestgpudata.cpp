#include "ptestgpudata.hpp"
#include <assert.h>
#include "CL/clext.h"
#include <iostream>

using namespace pt;
using namespace cl;
using namespace std;

PTestGPUData::PTestGPUData(const ContextDevQue* c) : cdq(c) 
{
	assert(cdq != NULL);
}

void PTestGPUData::createProgram(Program::Sources& s)
{
	program = Program(*cdq->context, s);	

	std::vector<Device> dvs;
	for (unsigned a = 0; a < cdq->devques.size(); a++)
		dvs.push_back(*cdq->devques[a].first);

	int err = program.build(dvs);
	clCheckError(err);
	if (err)
		std::cout 
			<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>()[0].second
			<< std::endl;

	kernel = Kernel(program, "p_test", &err);
	clCheckError(err);
}

void PTestGPUData::loadData
(
		unsigned inDataSize,
	   	float* firstValue,
	   	unsigned devicePermutations,
		unsigned prime,
		unsigned vectorSize,
		unsigned cutPoint
)
{
	devicePerm = devicePermutations;
	writeMem.clear();
	writeMem.push_back(Event());

	int err;
	inBuffer = Buffer(
			*cdq->context,
		   	CL_MEM_READ_WRITE, 
			sizeof(float) * prime,
			NULL,
			&err
			);	

	clCheckError(err);

	outBuffer = Buffer(
				*cdq->context,
				CL_MEM_READ_WRITE,
				sizeof(float) * devicePermutations * cdq->devques.size(),
				NULL,
				&err
				);
	clCheckError(err);

	extraOutBuffer = Buffer(
				*cdq->context,
				CL_MEM_READ_WRITE,
				sizeof(float) * devicePermutations * cdq->devques.size(),
				NULL,
				&err
				);
	clCheckError(err);

	err = cdq->devques[0].second->enqueueWriteBuffer
		(
			inBuffer,
			CL_FALSE,
			0,
			sizeof(float) * inDataSize,
			firstValue,
			NULL,
			&writeMem[0]
		);
	clCheckError(err);

	err = kernel.setArg(0, inBuffer);
	clCheckError(err);
	err = kernel.setArg(1, outBuffer);
	clCheckError(err);
	err = kernel.setArg(2, sizeof(unsigned), &prime);
	clCheckError(err);
	err = kernel.setArg(3, sizeof(unsigned), &vectorSize);
	clCheckError(err);
	err = kernel.setArg(4, sizeof(unsigned), &cutPoint);
	clCheckError(err);
	
	unsigned dataSize = inDataSize / vectorSize;
	err = kernel.setArg(5, sizeof(unsigned), &dataSize);
	clCheckError(err);

	err = kernel.setArg(6, extraOutBuffer);
	clCheckError(err);
}

void PTestGPUData::run(float* writeBackLocation, float* extraWriteBackLocation)
{
	execEvent.clear();
	readEvent.clear();
	int err;

	readEvent.push_back(Event());
	readEvent.push_back(Event());
	for (unsigned a = 0; a < cdq->devques.size(); a++)
		execEvent.push_back(Event());	

	for (unsigned b = 0; b < cdq->devques.size(); b++)
	{
		const CommandQueue* q(cdq->devques[b].second);
		err = q->enqueueNDRangeKernel
			(
				kernel,
				devicePerm * b,
				devicePerm,
				cl::NullRange,
				&writeMem,
				&execEvent[b]
			);
		clCheckError(err);
	}

	err = cdq->devques[0].second->enqueueReadBuffer
		(
			outBuffer,
			CL_FALSE,
			0,
			sizeof(float) * devicePerm * cdq->devques.size(),
			writeBackLocation,
			&execEvent,
			&readEvent[0]
		);	
	clCheckError(err);

	err = cdq->devques[0].second->enqueueReadBuffer
		(
			extraOutBuffer,
			CL_FALSE,
			0,
			sizeof(float) * devicePerm * cdq->devques.size(),
			extraWriteBackLocation,
			&execEvent,
			&readEvent[1]
		);	
	clCheckError(err);
}

void PTestGPUData::waitForEnd()
{
	assert(readEvent.size() >= 0);
	
//	assert(readEvent.size() != 2);
	readEvent[0].wait();
	readEvent[1].wait();
}
