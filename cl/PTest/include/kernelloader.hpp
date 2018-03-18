#pragma once
#define KERNEL_INFO_FILE "kernels_info.txt"
#include "kernelinfo.hpp"
#include <vector>

namespace pt
{
	class KernelLoader
	{
		public:
			static void initKernelLoader(const char* pathToFolder);
			static void clearKernelLoader();
			static const std::vector<KernelInfo>& getInfo();
			static bool areCompatible(const char* name, unsigned vectorSize);
			static void getPossibleStatistics(unsigned vecSize, std::vector<std::string>& v);
			static unsigned getMinVectorSize(const char* name);
			static unsigned getMaxVectorSize(const char* name);
			static std::string getProgram
				(
					const char* statistic, 
					bool useCPM, 
					unsigned vecSize,
					unsigned sampleSize,
					unsigned prime
				);
			static unsigned getNearestPrime(unsigned val);

		private:
			static KernelLoader* loader;
			KernelLoader(const char* pathToFolder);
			std::vector<KernelInfo> info;
			std::string folder;
			
	};
}
