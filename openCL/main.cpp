#include "PTest/kernelloader.hpp"
#include "PTest/ptest.hpp"
#include "PTest/environement.hpp"
#include <iostream>
#include <random>

int main()
{
	pt::ContextList indicies;
	indicies.push_back(pt::DevicesList());
	indicies[0].push_back(0);

	pt::KernelLoader::initKernelLoader("./kernels");	
	pt::Environement::initEnvironement(indicies);
	int changes(0);

	std::default_random_engine eng(time(0));
	std::uniform_real_distribution<float> dist(0, 1);
	std::uniform_real_distribution<float> dist2(0, 1);
	
	auto info(pt::KernelLoader::getInfo());
	for (int b = 0; b < 1000; b++)
	{
		pt::PTest loader(1, 1000, false, "ttest", 1000, 0.05f);
		loader.setCutPoint(500);
		for (int a = 0; a < 500; a++)
			loader.setValue(0, a, dist(eng));
		for (int a = 500; a < 1000; a++)
			loader.setValue(0, a, dist(eng));
		loader.runTest();

		changes += loader.isThereAChange();
	}
	std::cout << changes << " out of " << 1000 << " i noticed a change" << std::endl;

	pt::Environement::clearEnvironement();
	return 0;
}
