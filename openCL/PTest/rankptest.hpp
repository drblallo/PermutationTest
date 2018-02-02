#pragma once
#include "ptest.hpp"

namespace pt
{
	
	class RankPTest : public PTest
	{
		public:
			RankPTest(
					unsigned valuesCount, 
					bool trackLocationOfChange, 
					const char* statistic, 
					unsigned iterations,
					float alpha
				);
			virtual ~RankPTest();	

		protected:
			std::vector<float> postProcessValues;
			virtual std::vector<float>& getVectorToLoad();
			virtual void preProcess();	

		private:
			void sortIndexes(std::vector<unsigned>& indexes);
			void calculateRank(std::vector<unsigned>& indexes, int left, int right);
			void merge(std::vector<unsigned>& indexes, int left, int right, int center);
	};
}
