#include "PTest/kernelloader.hpp"
#include "PTest/ptest.hpp"
#include "PTest/environement.hpp"
#include <iostream>

int main()
{
	std::vector<int> indicies(0);
	indicies.push_back(0);
	pt::KernelLoader::initKernelLoader("./kernels");	
	pt::Environement::initEnvironement(indicies);
	
	auto info(pt::KernelLoader::getInfo());
	for (unsigned a = 0; a < info.size(); a++)
	{

		pt::PTest loader(1, false, info[a].getName().c_str());
		std::cout << info[a].getName() << "\n";	
	}

	pt::Environement::clearEnvironement();
	return 0;
}
