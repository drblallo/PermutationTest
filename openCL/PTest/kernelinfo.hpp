#pragma once
#include <string>

namespace pt
{
	class KernelInfo
	{
		public:
			KernelInfo(const char* name, unsigned minVectorSize, unsigned maxVectorSize);
			KernelInfo(std::string& name, unsigned minVectorSize, unsigned maxVectorSize);

			inline const std::string& getName() const
			{
				return name;
			}
			
			inline unsigned getMaxVectorSize() const
			{
				return maxVectorSize;
			}

			inline unsigned getMinVectorSize() const
			{
				return minVectorSize;
			}

			inline void setName(const char* nName)
			{
				name = std::string(nName);	
			}

			inline void setMinVectorSize(unsigned val)
			{
				minVectorSize = val;
			}

			inline void setMaxVectorSize(unsigned val)
			{
				maxVectorSize = val;
			}
			
		private:
			std::string name;
			unsigned minVectorSize;
			unsigned maxVectorSize;
	};
}
