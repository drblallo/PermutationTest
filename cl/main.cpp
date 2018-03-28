#include "kernelloader.hpp"
#include "ptest.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include "rankptest.hpp"


template <class T1, class T2, class T3, class T4, class T5, class T6>
void populate(pt::PTest& ptest, T1& d1, T2& d2, T3& d3, T4& d4, T5& d5, T6& d6, std::default_random_engine& eng)
{
	for (unsigned a = 0; a < ptest.getCutPoint(); a++)
	{
		ptest.setValue(0, a, d1(eng));
		ptest.setValue(1, a, d2(eng));
		ptest.setValue(2, a, d3(eng));
	}

	for (unsigned a = ptest.getCutPoint(); a < ptest.getSampleSize(); a++)
	{
		ptest.setValue(0, a, d4(eng));
		ptest.setValue(1, a, d5(eng));
		ptest.setValue(2, a, d6(eng));
	}
}

int runTests(unsigned vSize, unsigned sSize, unsigned iterationsCount, unsigned testsCount)
{
	int changes(0);
	std::default_random_engine eng(time(0));
	std::normal_distribution<float> dist(1, 2);
	std::normal_distribution<float> dist2(1, 4);
	std::normal_distribution<float> dist3(1, 1);
	std::normal_distribution<float> dist4(1, 1);
	std::normal_distribution<float> dist5(1, 1);
	std::normal_distribution<float> dist6(1, 1);

	//pt::RankPTest loader(sSize, "mann-whitney" /*"ttest"*/, 1000, 0.05f);
	//pt::PTest loader(1, sSize, "ttest", 1000, 0.05f);
	pt::PTest loader(vSize, sSize, "innerProduct", iterationsCount, 0.05f);
	loader.setCutPoint(sSize/2);
	for (int b = 0; b < testsCount; b++)
	{
		populate(loader, dist, dist2, dist3, dist4, dist5, dist6, eng);
		loader.runTest();
		changes += loader.isThereAChange();
	}
	std::cout << changes << " out of " << testsCount << " i noticed a change" << std::endl;
	return changes;

}

void runAllTests()
{
	std::ofstream f;
	f.open("./log/normal2-disequal.txt");
	unsigned vSize(3);
	unsigned sSize(1000);
	unsigned testsCount(1000);

	//for (int iterationsCount = 1000; iterationsCount == 1000; iterationsCount *= 2) {
	for (int iterationsCount = 2; iterationsCount < 10000; iterationsCount *= 2) {
		int success = runTests(vSize, sSize, iterationsCount, testsCount);
		std::cout << iterationsCount << std::endl;
		f << iterationsCount << " " << success << std::endl;
	}

}


#include <sstream>
void testSingle(pt::PTest& test, std::string& kernelName, std::string& name, bool first, bool second, int vcount)
{
	std::string line;
	std::ifstream infile(name + ".mat");
	int count(0);

	while (std::getline(infile, line))
	{
		std::vector<std::string> elems;

		std::istringstream iss(line);
		std::copy(std::istream_iterator<std::string>(iss),
			std::istream_iterator<std::string>(),
			std::back_inserter(elems));

		for (int a = 0; a < elems.size(); a++) {
			test.setValue(a, count, std::stof(elems[a]));
			//std::cout << std::stof(elems[a]) << std::endl;
		}

		count++;
	}
	test.setCutPoint(500);
	test.runTest();
	std::cout << test.isThereAChange() << " " << first << " " << second << std::endl;

	std::ofstream s;
	s.open("outputs/" + kernelName + "_" + name + ".txt", std::ios::app | std::ios::out);
	s << test.getIterations() << " " << test.isThereAChange() << " " << first << " " << second << std::endl;

	s.close();
}


void testSingle(pt::PTest& single, pt::PTest& doubleOne, std::string& kn)
{
	std::string line;
	std::ifstream infile("index.txt");
	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		std::string name, first, second, vCount;

		if (!(iss >> name >> first >> second >> vCount)) break;
		std::cout << line << std::endl;

		if (std::stoi(vCount) == 1)
			testSingle(single, kn, name, first.at(0) == 't', second.at(0) == 't', std::stoi(vCount));
		else
			testSingle(doubleOne, kn, name, first.at(0) == 't', second.at(0) == 't', std::stoi(vCount));

	}

}

void testAll()
{

	for (int a = 2; a < 10000; a = a * 2) {
		std::string s("vectorDifference");
		pt::PTest test(1, 1000, s.c_str(), a, 0.05f);
		pt::PTest test2(8, 1000, s.c_str() , a, 0.05f);
		testSingle(test, test2, s);

		s = "vectorDifference2";
		pt::PTest testC(1, 1000, s.c_str(), a, 0.05f);
		pt::PTest testC2(8, 1000, s.c_str(), a, 0.05f);
		testSingle(testC, testC2, s);

		s = "innerProduct";
		pt::PTest testI(1, 1000, s.c_str(), a, 0.05f);
		pt::PTest testI2(8, 1000, s.c_str(), a, 0.05f);
		testSingle(testI, testI2, s);

		s = "pairedProducts";
		pt::PTest testP(1, 1000, s.c_str(), a, 0.05f);
		pt::PTest testP2(8, 1000, s.c_str(), a, 0.05f);
		testSingle(testP, testP2, s);

		s = "vectorDifference3";
		pt::PTest testR(1, 1000, s.c_str(), a, 0.05f);
		pt::PTest testR2(8, 1000, s.c_str(), a, 0.05f);
		testSingle(testR, testR2, s);
	}
}

int main()
{
	pt::ContextList indicies;
	indicies.push_back(pt::DevicesList());
	indicies[0].push_back(0);
	
	pt::KernelLoader::initKernelLoader("./kernels");
	pt::Environement::initEnvironement(indicies);

	//runAllTests();
	testAll();

	pt::Environement::clearEnvironement();
	pt::KernelLoader::clearKernelLoader();
	return 0;
}
