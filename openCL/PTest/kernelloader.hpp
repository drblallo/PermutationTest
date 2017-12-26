#pragma once
#define KERNEL_INFO_FILE "kernels_info.txt"
#define CORE_KERNEL_FILE "core_kernel.cl"
#define CPM_KERNEL_FILE "cpm_kernel.cl"
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
			static void getPossibleStatistics(unsigned vecSize, std::vector<std::string>& v);
			static unsigned getMinVectorSize(const char* name);
			static unsigned getMaxVectorSize(const char* name);
			static std::string getProgram(const char* statistic, bool useCPM, unsigned vecSize);

		private:
			static KernelLoader* loader;
			KernelLoader(const char* pathToFolder);
			std::vector<KernelInfo> info;
			std::string folder;
			
	};
}
