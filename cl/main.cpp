#include "kernelloader.hpp"
#include "ptest.hpp"
#include <iostream>
#include <random>
#include "rankptest.hpp"

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
	unsigned vSize(3);
	unsigned sSize(1000);

	auto info(pt::KernelLoader::getInfo());
	for (int b = 0; b < 1000; b++)
	{
		//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
		//pt::PTest loader(1, sSize, "ttest", 1000, 0.05f);
		pt::PTest loader(vSize, sSize, "vectorDifference", 1000, 0.05f);
		loader.setCutPoint(sSize/2);
		for (unsigned a = 0; a < sSize/2; a++)
			for (unsigned b = 0; b < vSize; b++)
				loader.setValue(b, a, dist(eng));

		for (unsigned a = sSize/2; a < sSize; a++)
			for (unsigned b = 0; b < vSize; b++)
				loader.setValue(b, a, dist2(eng));

		loader.runTest();

		changes += loader.isThereAChange();
	}
	std::cout << changes << " out of " << 1000 << " i noticed a change" << std::endl;

	pt::Environement::clearEnvironement();
	return 0;
}
