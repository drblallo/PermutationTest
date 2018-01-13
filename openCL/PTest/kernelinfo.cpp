#include "kernelinfo.hpp"

using namespace pt;

KernelInfo::KernelInfo(const char* nm, unsigned mnVectorSize, unsigned mxVectorSize) :
	name(nm),
	minVectorSize(mnVectorSize),
	maxVectorSize(mxVectorSize)
{

}

KernelInfo::KernelInfo(std::string& nm, unsigned mnVectorSize, unsigned mxVectorSize) :
	name(nm),
	minVectorSize(mnVectorSize),
	maxVectorSize(mxVectorSize)
{

}
