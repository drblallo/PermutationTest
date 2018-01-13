#pragma once
#include "environement.hpp"
#include <vector>

namespace pt
{
	class PTestGPUData
	{
		public:
			PTestGPUData(const pt::ContextDevQue* c);

			void createProgram(cl::Program::Sources& s);
			void loadData(
					unsigned inDataSize,
				   	float* firstValue,
				   	unsigned devicePermutations,
					unsigned prime,
					unsigned vectorSize,
					unsigned cutPoint
					);
			void run(float* writeBackLocation);
			void waitForEnd();

			inline unsigned deviceCount() const {return cdq->devques.size();}

		private:
			const pt::ContextDevQue* cdq;
			cl::Buffer inBuffer;
			cl::Buffer outBuffer;
			cl::Program program;
			cl::Kernel kernel; 
			std::vector<cl::Event> writeMem;
			std::vector<cl::Event> execEvent;
			std::vector<cl::Event> readEvent;
			unsigned devicePerm;

	};
}