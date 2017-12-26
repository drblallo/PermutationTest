// auto generated from .clext.py
#ifndef __CL_EXT__
#define __CL_EXT__





/*
 * Given a cl code and return a string represenation
 */
const char* clGetErrorString(int errorCode);

/*
 * check cl error, if not CL_SUCCESS, print to stderr
 */
int clCheckError(int errorCode);

#endif /* __CL_EXT__*/
