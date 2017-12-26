#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include <math.h>
#include <random>

int initRandom = 0;
int randomNumber(int min, int max)
{
	if (initRandom == 0)
	{
		initRandom = 1;
		srand(time(NULL));
	}
	
	int r;

	const unsigned int range = 1 + max - min;
	const unsigned int buckets = RAND_MAX / range;
	const unsigned int limit = buckets * range;

	do
	{
		r = rand();
	} while (r >= limit);

	return min + (r/buckets);
}

float randomFloat()
{
	if (initRandom == 0)
	{
		initRandom = 1;
		srand(time(NULL));
	}

    return (float) rand() / (float) RAND_MAX;
}
//==============================================================================
// Recursive definition of determinate using expansion by minors.
//
// Notes: 1) arguments:
//             a (double **) pointer to a pointer of an arbitrary square matrix
//             n (int) dimension of the square matrix
//
//        2) Determinant is a recursive function, calling itself repeatedly
//           each time with a sub-matrix of the original till a terminal
//           2X2 matrix is achieved and a simple determinat can be computed.
//           As the recursion works backwards, cumulative determinants are
//           found till untimately, the final determinate is returned to the
//           initial function caller.
//
//        3) m is a matrix (4X4 in example)  and m13 is a minor of it.
//           A minor of m is a 3X3 in which a row and column of values
//           had been excluded.   Another minor of the submartix is also
//           possible etc.
//             m  a b c d   m13 . . . .
//                e f g h       e f . h     row 1 column 3 is elminated
//                i j k l       i j . l     creating a 3 X 3 sub martix
//                m n o p       m n . p
//
//        4) the following function finds the determinant of a matrix
//           by recursively minor-ing a row and column, each time reducing
//           the sub-matrix by one row/column.  When a 2X2 matrix is
//           obtained, the determinat is a simple calculation and the
//           process of unstacking previous recursive calls begins.
//
//                m n
//                o p  determinant = m*p - n*o
//
//        5) this function uses dynamic memory allocation on each call to
//           build a m X m matrix  this requires **  and * pointer variables
//           First memory allocation is ** and gets space for a list of other
//           pointers filled in by the second call to malloc.
//
//        6) C++ implements two dimensional arrays as an array of arrays
//           thus two dynamic malloc's are needed and have corresponsing
//           free() calles.
//
//        7) the final determinant value is the sum of sub determinants
//
//==============================================================================


float Determinant(float**a,int n)
{
    int i,j,j1,j2 ;                    // general loop and matrix subscripts
    float det = 0 ;                   // init determinant
    float **m = NULL ;                // pointer to pointers to implement 2d
                                       // square array

    if (n < 1)    {   }                // error condition, should never get here

    else if (n == 1) {                 // should not get here
        det = a[0][0] ;
        }

    else if (n == 2)  {                // basic 2X2 sub-matrix determinate
                                       // definition. When n==2, this ends the
        det = a[0][0] * a[1][1] - a[1][0] * a[0][1] ;// the recursion series
        }


                                       // recursion continues, solve next sub-matrix
    else {                             // solve the next minor by building a
                                       // sub matrix
        det = 0 ;                      // initialize determinant of sub-matrix

                                           // for each column in sub-matrix
        for (j1 = 0 ; j1 < n ; j1++) {
                                           // get space for the pointer list
            m = (float**) malloc((n-1)* sizeof(float*)) ;

            for (i = 0 ; i < n-1 ; i++)
                m[i] = (float*) malloc((n-1)* sizeof(float)) ;
                       //     i[0][1][2][3]  first malloc
                       //  m -> +  +  +  +   space for 4 pointers
                       //       |  |  |  |          j  second malloc
                       //       |  |  |  +-> _ _ _ [0] pointers to
                       //       |  |  +----> _ _ _ [1] and memory for
                       //       |  +-------> _ a _ [2] 4 doubles
                       //       +----------> _ _ _ [3]
                       //
                       //                   a[1][2]
                      // build sub-matrix with minor elements excluded
            for (i = 1 ; i < n ; i++) {
                j2 = 0 ;               // start at first sum-matrix column position
                                       // loop to copy source matrix less one column
                for (j = 0 ; j < n ; j++) {
                    if (j == j1) continue ; // don't copy the minor column element

                    m[i-1][j2] = a[i][j] ;  // copy source element into new sub-matrix
                                            // i-1 because new sub-matrix is one row
                                            // (and column) smaller with excluded minors
                    j2++ ;                  // move to next sub-matrix column position
                    }
                }

            det += pow(-1.0,1.0 + j1 + 1.0) * a[0][j1] * Determinant(m,n-1) ;
                                            // sum x raised to y power
                                            // recursively get determinant of next
                                            // sub-matrix which is now one
                                            // row & column smaller

            for (i = 0 ; i < n-1 ; i++) free(m[i]) ;// free the storage allocated to
                                            // to this minor's set of pointers
            free(m) ;                       // free the storage for the original
                                            // pointer to pointer
        }
    }
    return(det) ;
}

std::default_random_engine engine;
std::uniform_real_distribution<float> uniform1(0, 1);
std::uniform_real_distribution<float> uniform2(0.1f, 1.1f);

std::normal_distribution<float> dist1(0, 1);
std::normal_distribution<float> dist2(0.5f, 1);
std::normal_distribution<float> dist3(1, 1);
std::normal_distribution<float> dist4(2, 2);

std::chi_squared_distribution<float> chiDist(3.0);
std::chi_squared_distribution<float> chiDist2(4.0);

float randomFromGaussianMixture1()
{
	if (randomFloat() < 0.5f)
		return dist1(engine);
	else
		return dist2(engine);
}

float randomFromGaussianMixture2()
{
	if (randomFloat() < 0.5f)
		return dist3(engine);
	else
		return dist4(engine);
}

float randomFromUniform()
{
	return uniform1(engine);
}

float randomFromUniform2()
{
	return uniform2(engine);
}

float randomFromChiSquared1()
{
	return chiDist(engine);
}

float randomFromChiSquared2()
{
	return chiDist2(engine);
}

float randomFromNormal()
{
	return dist1(engine);
}

float randomFromNormal2()
{
	return dist2(engine);
}
