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
	typedef std::vector<int> DevicesList; 
	typedef std::vector<DevicesList> ContextList;

	//each device has a queue related to him, this is used to keep track of them
	class  DevQue
	{
		public:
			DevQue(cl::Device* d, cl::CommandQueue* q) : device(d),  commandQueue(q) {}
			cl::Device* getDevice() const {return device;}
			cl::CommandQueue* getQueue() const {return commandQueue;}
			void setQueue(cl::CommandQueue* q) {commandQueue = q;} 

		private:
			cl::Device* device;
			cl::CommandQueue* commandQueue;

	};

	//a collecetion of devices can be part of a context, this is used to keep
	//treack of them
	class ContextDevQue
	{
		public:
			ContextDevQue(cl::Context& con) : context(&con) {}
			ContextDevQue(cl::Context& con, std::vector<DevQue>& dq) : 
				context(&con),
				devques(dq)
			{}
			cl::Context* getContex() const {return context;}
			const DevQue* getDevQueue(unsigned index) const {return &devques[index];}
			DevQue* getDevQueue(unsigned index){return &devques[index];}
			unsigned getDevQueueSize() const {return devques.size();}
			void addDevQueue(DevQue d) {devques.push_back(d);}

		private:
			cl::Context* context;
			std::vector<DevQue> devques;
	};

	//a enviroment is a singleton object that keeps track of
	//every context queue and device.
	class Environement
	{
		public:
			static bool initEnvironement(ContextList& devicesIndex);
			static void clearEnvironement();
			static void printAllDevices();

			static const std::vector<cl::Platform>* getPlatforms();
			static const std::vector<ContextDevQue>* getContextDevQue();

			static unsigned getContextSize();
			static unsigned getQueuesSize();
			static bool initExecuted() {return environement != NULL;}

		private:
			Environement(bool &success, ContextList& devincesIndex);
			static Environement* environement;

			bool discoverPlatforms();
			bool discoverDevices(ContextList& devicesIndex);
			bool createContextes(ContextList& devicesIndex);

			std::vector<cl::CommandQueue> queues;
			std::vector<cl::Context> contextes;
			std::vector<cl::Device> devices;
			std::vector<cl::Platform> platforms;
			std::vector<ContextDevQue> contexDevQueues;
	};
}
