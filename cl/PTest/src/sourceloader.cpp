#include "sourceloader.hpp"
#include <iostream>
#include <assert.h>
#include <fstream>

using namespace pt;
using namespace std;

SourceLoader::SourceLoader(std::string folderName, std::string name, bool useCPM)
{
	std::string corePath;
	if (!useCPM)
		corePath = folderName + CORE_KERNEL_FILE; //path to the core file in normal mode
	else
		corePath = folderName + CPM_KERNEL_FILE; //path to the core file in cpm mode

	string statisticPath(folderName + name+ ".cl"); //path to statistis file
	string utilsPath(folderName + UTILS_FILE);// path to util

	ifstream coreFile(corePath);
	ifstream sFile(statisticPath);
	ifstream utilsFile(utilsPath);

	assert (coreFile.good() == true);
	assert (sFile.good() == true);
	assert (utilsFile.good() == true);

	string core((istreambuf_iterator<char>(coreFile)), (istreambuf_iterator<char>()));
	string stat((istreambuf_iterator<char>(sFile)), (istreambuf_iterator<char>()));
	string utils((istreambuf_iterator<char>(utilsFile)), (istreambuf_iterator<char>()));

	text = utils;
	text += stat + core;
}

void SourceLoader::replaceEveryOccurence(const std::string &from, const std::string &to)
{
	size_t start_pos = 0;
	while((start_pos = text.find(from, start_pos)) != std::string::npos)
	{
		text.replace(start_pos, from.length(), to);
		start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
	}
}

void SourceLoader::replaceEveryOccurence(const std::string &from, float into)
{
	std::string s(std::to_string(into));
	replaceEveryOccurence(from, s);
}

void SourceLoader::replaceEveryOccurence(const std::string &from, int into)
{
	std::string s(std::to_string(into));
	replaceEveryOccurence(from, s);
}

void SourceLoader::replaceEveryOccurence(const std::string &from, unsigned into)
{
	std::string s(std::to_string(into));
	replaceEveryOccurence(from, s);
}
