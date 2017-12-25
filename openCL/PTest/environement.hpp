#pragma once
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_SIZE_T_COMPATIBILITY
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY
#define CL_HPP_CL_1_2_DEFAULT_BUILD
#include "CL/cl2.hpp"
#include <vector>

namespace pt
{
	class Environement
	{
		public:
			static bool initEnvironement(std::vector<int>& devicesIndex);
			static void clearEnvironement();

			static const std::vector<cl::Platform>* getPlatforms();
			static const std::vector<cl::CommandQueue>* getQueues();
			static const std::vector<cl::Context>* getContextes();
			static const std::vector<cl::Device>* getDevices();

		private:
			Environement(bool &success, std::vector<int>& devincesIndex);
			static Environement* environement;

			bool discoverPlatforms();
			bool discoverDevices(std::vector<int>& devicesIndex);

			std::vector<cl::CommandQueue> queues;
			std::vector<cl::Context> contextes;
			std::vector<cl::Device> devices;
			std::vector<cl::Platform> platforms;
	};
}
