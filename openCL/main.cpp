#include "PTest/kernelloader.hpp"
#include "PTest/ptest.hpp"
#include "PTest/environement.hpp"
#include <iostream>

int main()
{
	pt::ContextList indicies;
	indicies.push_back(pt::DevicesList());
	indicies[0].push_back(0);

	pt::KernelLoader::initKernelLoader("./kernels");	
	pt::Environement::initEnvironement(indicies);
	
	auto info(pt::KernelLoader::getInfo());
	pt::PTest loader(1, 1000, false, "ttest", 1000);
	loader.runTest();
	loader.runTest();
	std::cout << loader.getChangeLocation() << "\n";	

	loader = pt::PTest(1, 1000, false, "ttest", 1000);
	loader.runTest();
	std::cout << loader.getChangeLocation() << "\n";	

	pt::Environement::clearEnvironement();
	return 0;
}
