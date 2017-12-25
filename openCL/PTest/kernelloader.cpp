#include "kernelloader.hpp"
#include <assert.h>
#include <fstream>

using namespace pt;
using namespace std;

KernelLoader* KernelLoader::loader(NULL);

void KernelLoader::initKernelLoader(const char* pathToFolder)
{
	if (!loader)
		loader = new KernelLoader(pathToFolder);
}

KernelLoader::KernelLoader(const char* pathToFolder) : info()
{
	ifstream file;	
	string fileName(pathToFolder);
	string name;
	string supp;
	unsigned min, max;
		
	if (fileName[fileName.size()] != '/') 
		fileName += '/'; //folders must end with a /

	fileName += KERNEL_INFO_FILE; //create information file path

	file.open(fileName); 
	
	assert(file.good() == true); 

	while (!file.eof())
	{
		if (file.peek() == '#') //if the line start with a #, then it's a comment line
		{
			getline(file, name);
			continue;
		}

		getline(file, name, ' '); //else the first part is the name of the statistic
		getline(file, supp, ' '); //the second part is the minimum vector size
		min = stoi(supp);
		getline(file, supp); //the thrid element is the maximum vector size

		max = stoi(supp);
		info.push_back(KernelInfo(name, min, max)); //create the info and add it to the list
	}

	file.close();
}

const std::vector<KernelInfo>& KernelLoader::getInfo()
{
	assert(loader != NULL);
	return loader->info;
}

bool KernelLoader::areCompatible(const char* name, unsigned vectorSize)
{
	assert(loader != NULL);
	auto info(loader->info);
	for (unsigned a = 0; a < info.size(); a++)
		if (info[a].getName().compare(name) == 0)	
			if  (
				info[a].getMaxVectorSize() >= vectorSize && 
				info[a].getMinVectorSize() <= vectorSize
				)
				return true;
	return false;
}

void KernelLoader::getPossibleStatistics(unsigned vectorSize, vector<string>& v)
{
	assert(loader != NULL);

	v.clear();	
	
	auto info(loader->info);
	for (unsigned a = 0; a < info.size(); a++)
		if	(
			info[a].getMaxVectorSize() >= vectorSize && 
			info[a].getMinVectorSize() <= vectorSize
			)
			v.push_back(string(info[a].getName()));
}

unsigned KernelLoader::getMinVectorSize(const char* name)
{
	assert(loader != NULL);

	auto info(loader->info);
	for (unsigned a = 0; a < info.size(); a++)
		if (info[a].getName().compare(name) == 0)	
			return info[a].getMinVectorSize();

	return 0;
}

unsigned KernelLoader::getMaxVectorSize(const char* name)
{
	assert(loader != NULL);

	auto info(loader->info);
	for (unsigned a = 0; a < info.size(); a++)
		if (info[a].getName().compare(name) == 0)	
			return info[a].getMaxVectorSize();

	return 0;
}
