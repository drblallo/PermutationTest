#include "gtest/gtest.h"
#include "pttest.hpp"

using namespace pt;

TEST_F(PTTest, pt_test)
{
	unsigned vSize(1);
	unsigned sSize(1000);
	auto info(pt::KernelLoader::getInfo());

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	pt::PTest loader(vSize, sSize, "ttest", 1000, 0.05f);
	//pt::PTest loader(vSize, sSize, "vectorDifference", 1000, 0.05f);
	EXPECT_EQ(loader.getVectorSize(), vSize);
	EXPECT_EQ(loader.getSampleSize(), sSize);
	loader.setCutPoint(sSize/2);
	for (unsigned a = 0; a < sSize/2; a++)
		for (unsigned b = 0; b < vSize; b++) {
			loader.setValue(b, a, 2);
			EXPECT_EQ(loader.getValue(b, a), 2);
		}

	for (unsigned a = sSize/2; a < sSize; a++)
		for (unsigned b = 0; b < vSize; b++) {
			loader.setValue(b, a, 0);
			EXPECT_EQ(loader.getValue(b, a), 0);
		}

	loader.runTest();

	EXPECT_EQ(loader.isThereAChange(), true);
}

TEST_F(PTTest, gaussian_test)
{
	unsigned vSize(1);
	unsigned sSize(1000);
	auto info(pt::KernelLoader::getInfo());

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	pt::PTest loader(vSize, sSize, "ttest", 1000, 0.05f);
	//pt::PTest loader(vSize, sSize, "vectorDifference", 1000, 0.05f);
	EXPECT_EQ(loader.getVectorSize(), vSize);
	EXPECT_EQ(loader.getSampleSize(), sSize);
	std::default_random_engine eng(0);
	std::normal_distribution<float> dist(2, 1);
	loader.setCutPoint(sSize/2);

	float count = 0;
	for (int a = 0; a < 1000; a++) {
		for (unsigned a = 0; a < sSize / 2; a++)
			for (unsigned b = 0; b < vSize; b++) {
				loader.setValue(b, a, dist(eng));
			}

		for (unsigned a = sSize / 2; a < sSize; a++)
			for (unsigned b = 0; b < vSize; b++) {
				loader.setValue(b, a, dist(eng));
			}

		loader.runTest();
		count += loader.isThereAChange();
	}

	EXPECT_NEAR(count/1000, 0.05, 0.02);
}

TEST_F(PTTest, example)
{
	unsigned vSize(1);
	unsigned sSize(1000);
	auto info(pt::KernelLoader::getInfo());

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	pt::PTest loader(vSize, sSize, "example", sSize, 0.05f);
	//pt::PTest loader(vSize, sSize, "vectorDifference", 1000, 0.05f);
	EXPECT_EQ(loader.getVectorSize(), vSize);
	EXPECT_EQ(loader.getSampleSize(), sSize);
	std::default_random_engine eng(time(0));
	std::normal_distribution<float> dist(2, 1);
	loader.setCutPoint(sSize/2);

	float count = 0;
	for (int a = 0; a < 1000; a++) {
		for (unsigned a = 0; a < sSize / 2; a++)
			for (unsigned b = 0; b < vSize; b++) {
				loader.setValue(b, a, dist(eng));
			}

		for (unsigned a = sSize / 2; a < sSize; a++)
			for (unsigned b = 0; b < vSize; b++) {
				loader.setValue(b, a, dist(eng));
			}

		loader.runTest();
		count += loader.isThereAChange();
	}

	std::cout << count/1000.0f << std::endl;
	EXPECT_NEAR(count/1000, 0.05f, 0.02f);

}

TEST_F(PTTest, multivariate)
{
	unsigned vSize(3);
	unsigned sSize(1000);

	auto info(pt::KernelLoader::getInfo());

	pt::PTest loader(vSize, sSize, "vectorDifference", 1000, 0.05f);
	EXPECT_EQ(loader.getVectorSize(), vSize);
	EXPECT_EQ(loader.getSampleSize(), sSize);
	loader.setCutPoint(sSize/2);
	for (unsigned a = 0; a < sSize/2; a++)
		for (unsigned b = 0; b < vSize; b++)
			loader.setValue(b, a, 2);

	for (unsigned a = sSize/2; a < sSize; a++)
		for (unsigned b = 0; b < vSize; b++)
			loader.setValue(b, a, 0);

	loader.runTest();
	EXPECT_EQ(loader.isThereAChange(), true);

	loader.setCutPoint(sSize/2);
	for (unsigned a = 0; a < sSize/2; a++)
		for (unsigned b = 0; b < vSize; b++)
			loader.setValue(b, a, 0);

	for (unsigned a = sSize/2; a < sSize; a++)
		for (unsigned b = 0; b < vSize; b++)
			loader.setValue(b, a, 0);

	loader.runTest();
	EXPECT_EQ(loader.isThereAChange(), false);
}
