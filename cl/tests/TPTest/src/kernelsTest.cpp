//
// Created by massimo on 09/04/18.
//

#include "pttest.hpp"

using namespace pt;
TEST_F(PTTest, pairedProducts)
{
	unsigned vSize(1);
	unsigned sSize(1000);
	auto info(pt::KernelLoader::getInfo());

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	pt::PTest loader(vSize, sSize, "pairedProducts", sSize, 0.05f);
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
TEST_F(PTTest, covariance)
{
	unsigned vSize(1);
	unsigned sSize(1000);
	auto info(pt::KernelLoader::getInfo());

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	pt::PTest loader(vSize, sSize, "covariance", sSize, 0.05f);
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
