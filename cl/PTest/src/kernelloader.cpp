#include "kernelloader.hpp"
#include <assert.h>
#include <fstream>
#include <algorithm>
#include "sourceloader.hpp"

using namespace pt;
using namespace std;

KernelLoader* KernelLoader::loader(NULL);

void KernelLoader::initKernelLoader(const char* pathToFolder)
{
	if (!loader)
		loader = new KernelLoader(pathToFolder);
}

KernelLoader::KernelLoader(const char* pathToFolder) : info(), folder()
{
	ifstream file;	
	string fileName(pathToFolder);
	string name;
	string supp;
	unsigned min, max;
		
	if (fileName[fileName.size()] != '/') 
		fileName += '/'; //folders must end with a /

	folder = fileName;
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

//returns true if the VECTORSIZE of a particular statistic NAME is among the allowed one
//by the config file
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

void KernelLoader::clearKernelLoader()
{
	if (loader == NULL)
	{
		delete loader;
		loader = NULL;
	}
}

//returns the list of names of statistics that can be used with a particular
//vector size, v will be cleared.
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

//return the minimum vector size of a particular statistic
unsigned KernelLoader::getMinVectorSize(const char* name)
{
	assert(loader != NULL);

	auto info(loader->info);
	for (unsigned a = 0; a < info.size(); a++)
		if (info[a].getName().compare(name) == 0)	
			return info[a].getMinVectorSize();

	return 0;
}


//build the right program for a particular statistic 
//the program is made of a core file, a util file and 
//a statistic file. 
string KernelLoader::getProgram
	(
		const char* satistic, 
		bool useCPM, 
		unsigned vectorSize,
		unsigned sampleSize,
		unsigned prime
	)
{
	assert(areCompatible(satistic, vectorSize) == true);

	SourceLoader ld(loader->folder, satistic, useCPM);
	ld.replaceEveryOccurence("OVER_BOUND_SIZE", prime - sampleSize);
	ld.replaceEveryOccurence("VECTOR_SIZE", vectorSize);
	ld.replaceEveryOccurence("SAMPLE_SIZE", sampleSize);
	if (useCPM)
		ld.replaceEveryOccurence("CPM_START_POINT", 20);

	return ld.getSource();
}

//return the max vector size for a particular statisic
unsigned KernelLoader::getMaxVectorSize(const char* name)
{
	assert(loader != NULL);

	auto info(loader->info);
	for (unsigned a = 0; a < info.size(); a++)
		if (info[a].getName().compare(name) == 0)	
			return info[a].getMaxVectorSize();

	return 0;
}

bool IsPrime(int number)
{

	if (number == 2 || number == 3)
		return true;

	if (number % 2 == 0 || number % 3 == 0)
		return false;

	int divisor = 6;
	while (divisor * divisor - 2 * divisor + 1 <= number)
	{
		if (number % (divisor - 1) == 0)
			return false;

		if (number % (divisor + 1) == 0)
			return false;

		divisor += 6;

	}

	return true;
}

unsigned KernelLoader::getNearestPrime(unsigned val)
{
	while (!((val % 4) == 3) ||!IsPrime(val) )
		val++;
	return val;
}
