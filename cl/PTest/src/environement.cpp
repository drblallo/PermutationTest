#include "environement.hpp"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include "clext.h"

using namespace pt;
using namespace cl;

Environement* Environement::environement(nullptr);

void Environement::clearEnvironement()
{
	assert(environement != NULL);
	delete environement;
	environement = NULL;
}

bool Environement::discoverPlatforms()
{
	Platform::get(&platforms);
	if (platforms.size() == 0)
	{
		std::cout << "No Platform found, check your opencl installation" << std::endl;
		return false;
	}
	std::cout << "found " << platforms.size() << " platforms" << std::endl;
	return true;
}

void Environement::printAllDevices()
{
	std::vector<cl::Platform> platforms;
	Platform::get(&platforms);
	int count(0);
	for (unsigned b = 0; b < platforms.size(); b++)
	{
		std::vector<cl::Device> devices;
		platforms[b].getDevices(CL_DEVICE_TYPE_ALL, &devices);

		for (unsigned a = 0; a < devices.size(); a++)
		{
			std::cout
					<< "device index: "
					<< count << ": is "
					<< devices[a].getInfo<CL_DEVICE_NAME>()
					<< ", "
					<< devices[a].getInfo<CL_DEVICE_OPENCL_C_VERSION>()
					<< std::endl;
			count++;
		}
	}

}

bool Environement::discoverDevices(ContextList& devicesIndex)
{

	std::vector<Device> dev;	
	int deviceCount(0);
	for (unsigned b = 0; b < platforms.size(); b++)
	{
		platforms[b].getDevices(CL_DEVICE_TYPE_ALL, &dev);
		
		for (unsigned a = 0; a < dev.size(); a++)
		{
			devices.push_back(dev[a]);
			deviceCount++;
		}
	}
	

	if (devices.size() > 0)
		return true;

	if (devicesIndex.size() > 0)
		std::cout << "could not select any device, either the list provided was incorrect or opencl installation is incorrect" << std::endl;

	return false;
}

const vector<Platform>* Environement::getPlatforms()
{
	if (environement == NULL)
		return NULL;
	return &environement->platforms;
}

const vector<ContextDevQue>* Environement::getContextDevQue()
{
	if (environement == NULL)
		return NULL;
	return &environement->contexDevQueues;
}

unsigned Environement::getContextSize()
{
	if (environement == NULL)
		return -1;

	return environement->contexDevQueues.size();
}

unsigned Environement::getQueuesSize()
{
	if (environement == NULL)
		return -1;

	unsigned quantity(0);
	for (unsigned b = 0; b < environement->contexDevQueues.size(); b++)
		quantity += environement->contexDevQueues[b].getDevQueueSize();

	return quantity;
}

Environement::Environement(bool &success, ContextList& devicesIndex) :
	queues(0),
	contextes(0),
	devices(0),
	platforms(0)
{
	cl_int error;
	success = true;
	if (!discoverPlatforms())
	{
		success = false;
		return;
	}	

	if (!discoverDevices(devicesIndex))
	{
		success = false;
		return;
	}

	if (!createContextes(devicesIndex))
	{
		success = false;
		return;
	}

	std::cout << "creating queues" << std::endl;

	unsigned quantity(0);
	for (unsigned b = 0; b < contexDevQueues.size(); b++)
		quantity += contexDevQueues[b].getDevQueueSize();

    queues.resize(quantity);
	int t(0);
	for (unsigned a = 0; a < contexDevQueues.size(); a++)
	{
		std::cout << "for context number: " << a << std::endl;
        auto cdq(&contexDevQueues[a]);
        for (unsigned b = 0; b < cdq->getDevQueueSize(); b++)
		{
			std::cout 
				<< "--for device: " 
                << cdq->getDevQueue(b)->getDevice()->getInfo<CL_DEVICE_NAME>()
				<< std::endl;

            queues[t] = CommandQueue(*cdq->getContex(), *cdq->getDevQueue(b)->getDevice(), 0, &error);
            cdq->getDevQueue(b)->setQueue(&queues[t]);
			t++;
			clCheckError(error);
		}
	}
}

bool Environement::createContextes(ContextList& devIndex)
{

		cl_int error;
		bool foundOne(false);
		for (unsigned a = 0; a < devIndex.size(); a++)
		{
			vector<cl::Device> lDev;
			std::cout << "trying to create context populated by:" << std::endl;
			for (unsigned b = 0; b < devIndex[a].size(); b++)
			{
				lDev.push_back(devices[devIndex[a][b]]);
				std::cout 
					<< "--device index: "
					<< devIndex[a][b] << ": is " 
					<< lDev.back().getInfo<CL_DEVICE_NAME>() 
					<< ", " 
					<< lDev.back().getInfo<CL_DEVICE_OPENCL_C_VERSION>() 
					<< std::endl;
			}

			contextes.push_back(Context(lDev, NULL, NULL, NULL, &error));
			clCheckError(error);
			if (error == 0)
			{
				foundOne = true;
				ContextDevQue cdq(contextes.back());
				for (unsigned b = 0; b < devIndex[a].size(); b++)
					cdq.addDevQueue(DevQue(&devices[devIndex[a][b]], NULL));

				contexDevQueues.push_back(cdq);
			}
		}

		if (!foundOne)
		{
			std::cout << "could not create context" << std::endl;
			return false;
		}

		return true;
}

bool Environement::initEnvironement(ContextList& devicesIndex)
{
    if (devicesIndex.size() == 0)
		return false;

	if (!environement)
	{
		bool success(true);
		environement = new Environement(success, devicesIndex);
		return success;
	}

	return true;
}
