#include "gtest/gtest.h"
#include "kernelloader.hpp"

using namespace pt;

TEST(basic_check, kernel_loader_init)
{
	KernelLoader::initKernelLoader("./kernels");

	EXPECT_EQ(KernelLoader::areCompatible("example", 0), false);
	EXPECT_EQ(KernelLoader::areCompatible("example", 1), true);
	EXPECT_EQ(KernelLoader::areCompatible("example", 11), false);

	KernelLoader::getProgram("example", false, 1, 100, 101);

	KernelLoader::clearKernelLoader();
}
