#pragma once
#include <string>
#define CORE_KERNEL_FILE "core_kernel.cl"
#define CPM_KERNEL_FILE "cpm_kernel.cl"
#define UTILS_FILE "utils.cl"

namespace pt
{
	class SourceLoader
	{
		public:
			SourceLoader(std::string folderName, std::string name, bool useCPM);
			void replaceEveryOccurence(const std::string& from, const std::string& into);
			void replaceEveryOccurence(const std::string& from, float into);
			void replaceEveryOccurence(const std::string& from, int into);
			void replaceEveryOccurence(const std::string& from, unsigned into);
			std::string getSource() const {return text;}

		private:
			std::string text;
	};
}
