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
			static const std::vector<KernelInfo>& getInfo();
			static bool areCompatible(const char* name, unsigned vectorSize);
			static void getPossibleStatistics(unsigned vectorSize, std::vector<std::string>& v);
			static unsigned getMinVectorSize(const char* name);
			static unsigned getMaxVectorSize(const char* name);

		private:
			static KernelLoader* loader;
			KernelLoader(const char* pathToFolder);
			std::vector<KernelInfo> info;
			
	};
}
