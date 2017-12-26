#include "ptestgpudata.hpp"
#include <assert.h>
#include "CL/clext.h"

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

	kernel = Kernel(program, "p_test", &err);
	clCheckError(err);
}

void PTestGPUData::loadData(unsigned inDataSize, float* firstValue, unsigned devicePermutations)
{
	devicePerm = devicePermutations;
	writeMem.clear();
	writeMem.push_back(Event());

	int err;
	inBuffer = Buffer(
			*cdq->context,
		   	CL_MEM_READ_WRITE, 
			sizeof(float) * inDataSize,
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

}

void PTestGPUData::run(float* writeBackLocation)
{
	execEvent.clear();
	readEvent.clear();
	int err;

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
}

void PTestGPUData::waitForEnd()
{
	assert(readEvent.size() >= 0);
	
	readEvent[0].wait();
}
