#include "environement.hpp"
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <CL/clext.h>

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

bool Environement::discoverDevices(std::vector<int>& devicesIndex)
{

	std::vector<Device> dev;	
	int deviceCount(0);
	for (unsigned b = 0; b < platforms.size(); b++)
	{
		platforms[b].getDevices(CL_DEVICE_TYPE_ALL, &dev);
		
		for (unsigned a = 0; a < dev.size(); a++)
		{
			if (std::find(devicesIndex.begin(), devicesIndex.end(), deviceCount) != devicesIndex.end())
				devices.push_back(dev[a]);
			if (devicesIndex.size() == 0)
				std::cout << "device index: " << deviceCount << ": is " << dev[a].getInfo<CL_DEVICE_NAME>() << ", " << dev[a].getInfo<CL_DEVICE_OPENCL_C_VERSION>() << std::endl;
		
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
	assert(environement != NULL);
	return &environement->platforms;
}

const vector<CommandQueue>* Environement::getQueues()
{
	assert(environement != NULL);
	return &environement->queues;
}

const vector<Device>* Environement::getDevices()
{
	assert(environement != NULL);
	return &environement->devices;
}

const vector<Context>* Environement::getContextes()
{
	assert(environement != NULL);
	return &environement->contextes;
}

Environement::Environement(bool &success, std::vector<int>& devicesIndex) :
	queues(0),
	contextes(0),
	devices(0),
	platforms(0)
{
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

	cl_int error;
	contextes.push_back(Context(devices, NULL, NULL, NULL, &error));
	
	clCheckError(error);
	if (error != 0)
	{
		success = false;	
		std::cout << "could not create context\n";
		return;
	}

	for (unsigned int a = 0; a < devices.size(); a++)
	{
		queues.push_back(CommandQueue(contextes[0], devices[a], 0, &error));
		clCheckError(error);
	}
}

bool Environement::initEnvironement(std::vector<int>& devicesIndex)
{
	if (!environement)
	{
		bool success(true);
		environement = new Environement(success, devicesIndex);
		return success;
	}

	return true;
}
