#pragma once
#include <vector>
#include <assert.h>
#include <string>
#include "environement.hpp"
#include "ptestgpudata.hpp"

namespace pt
{
	//A PTest how a user will interact with the library
	//When a PTest object is created it is initializaed with the size
	//of the vector and the statistic used.
	//
	//Then data can be loaded with setValue and the test can be executed.
	//Once the test is compleated info about the success or the failure can
	//be read.
	class PTest 
	{
		public:
			PTest(unsigned vectorSize, unsigned valuesCount, bool trackLocationOfChange, const char* statistic, unsigned iterations);
			~PTest();

			/////////////////////////////////////////
			//Copy assignament and copy constructor//
			/////////////////////////////////////////
			PTest(const PTest& other);
			PTest& operator=(const PTest& other);
			
			
			/////////////////////////////////////////
			//           setter and getter         //
			/////////////////////////////////////////
			inline void setValue(unsigned inVectorLocation, unsigned pos, float value)
			{
				unsigned index((pos * vectorSize) + inVectorLocation);
				assert (inVectorLocation < vectorSize);
				if (index >= values.size())
					values.resize(index + 1);
				values[index] = value;
				mustLoadData = true;
			}

			inline void resizeData(unsigned newSize)
			{
				values.resize((newSize * vectorSize) + 1);
			}

			inline float getValue(unsigned inVectorLocation, unsigned pos) const
			{
				unsigned index((pos * vectorSize) + inVectorLocation);
				assert (index < values.size());
				assert (inVectorLocation < vectorSize);
				return values[index];
			}

			inline bool isThereAChange() const
			{
				assert(testExecuted == true);
				return changeFound;
			}	

			inline unsigned getChangeLocation() const
			{
				assert(testExecuted == true);
				return changeLocation;
			}

			inline bool isTestCompleated() const 
			{
				return testExecuted;
			}

			void runTest();

		private:
			/////////////////////////////////////////
			//              cpu data               //
			/////////////////////////////////////////
			std::vector<float> values;
			std::vector<float> outValues;
			bool mustCreateProgram;
			bool mustLoadData;
			unsigned vectorSize;
			bool changeFound;
			unsigned changeLocation;
			bool testExecuted;
			bool useCPM;
			unsigned iterations;
			std::string statisticName;

			/////////////////////////////////////////
			//              gpu data               //
			/////////////////////////////////////////

			std::vector<PTestGPUData> gpuData;
			void setUpGPUData();
			void createProgram();
			void loadData();
	};
}
