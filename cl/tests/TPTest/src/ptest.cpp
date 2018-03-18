#include "gtest/gtest.h"
#include "ptest.hpp"
#include "kernelloader.hpp"
#include "environement.hpp"

using namespace pt;

struct PTTest : testing::Test
{
	PTTest()
	{
		ContextList indicies;
		indicies.push_back(pt::DevicesList());
		indicies[0].push_back(0);

		KernelLoader::initKernelLoader("./kernels");
		Environement::initEnvironement(indicies);
	}

	~PTTest()
	{
		Environement::clearEnvironement();
		KernelLoader::clearKernelLoader();
	}
};

TEST_F(PTTest, pt_test)
{
	unsigned vSize(1);
	unsigned sSize(1000);

	auto info(pt::KernelLoader::getInfo());

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	pt::PTest loader(1, sSize, "ttest", 1000, 0.05f);
	//pt::PTest loader(vSize, sSize, "vectorDifference", 1000, 0.05f);
	loader.setCutPoint(sSize/2);
	for (unsigned a = 0; a < sSize/2; a++)
		for (unsigned b = 0; b < vSize; b++)
			loader.setValue(b, a, 2);

	for (unsigned a = sSize/2; a < sSize; a++)
		for (unsigned b = 0; b < vSize; b++)
			loader.setValue(b, a, 0);

	loader.runTest();

	EXPECT_EQ(loader.isThereAChange(), true);
}