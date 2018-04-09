//
// Created by massimo on 09/04/18.
//

#ifndef TESTPROJECT_PTTEST_HPP
#define TESTPROJECT_PTTEST_HPP

#include "ptest.hpp"
#include "kernelloader.hpp"
#include "environement.hpp"
#include "gtest/gtest.h"

struct PTTest : testing::Test
{
	PTTest()
	{
		pt::ContextList indicies;
		indicies.push_back(pt::DevicesList());
		indicies[0].push_back(0);

		pt::KernelLoader::initKernelLoader("./kernels");
		pt::Environement::initEnvironement(indicies);
	}

	~PTTest()
	{
		pt::Environement::clearEnvironement();
		pt::KernelLoader::clearKernelLoader();
	}
};
#endif //TESTPROJECT_PTTEST_HPP
