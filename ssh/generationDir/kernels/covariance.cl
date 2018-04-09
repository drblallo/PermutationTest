unsigned int rand(unsigned int* seed, int sampleSize) // 1 <= *seed < m
{
    unsigned int const a = 16807; //ie 7**5

    *seed = (*seed * a) % sampleSize;
    return(*seed);
}



void calculateCovariance(__global const float* in, float* matrix, int start, int end, int* displ)
{
    float meanX[VECTOR_SIZE];

    for (int a = 0; a < VECTOR_SIZE; a++)
        meanX[a] = 0;

    for (int x = 0; x < VECTOR_SIZE; x++)
    {
        for (int n = start; n < end; n++)
        {
           meanX[x] += in[(VECTOR_SIZE * displ[n]) + x];
        }
        meanX[x] = meanX[x] / (end - start);
    }

    for (int x = 0; x < VECTOR_SIZE; x++)
    {
        for (int y = 0; y < VECTOR_SIZE; y++)
        {
            for (int n = start; n < end; n++)
            {
                matrix[(VECTOR_SIZE * x) + y] += (in[(VECTOR_SIZE * displ[n]) + x]) * (in[(VECTOR_SIZE * displ[n]) + y]);
            }
            matrix[(VECTOR_SIZE * x) + y] /= end - start - 1;
        }
    }
}

float evaluateStatistic
(
	__global const float* in,
	unsigned int index,
	const unsigned int prime,
	unsigned int sd,
	const unsigned int offset,
	const unsigned int sampleSize,
	const unsigned int cutPoint,
	const unsigned int vectorSize
)
{

    int displacement[SAMPLE_SIZE];
    unsigned int seed = sd;

    for (int a = 0; a < SAMPLE_SIZE; a++)
        displacement[a] = a;

    for (int a = 0; a < SAMPLE_SIZE; a++)
    {
        int newLoc = rand(&seed, sampleSize);
        int supp = displacement[a];
        if (index != 0){
            displacement[a] = displacement[newLoc];
            displacement[newLoc] = supp;
        }
    }

    float covariance1[VECTOR_SIZE * VECTOR_SIZE];
    float covariance2[VECTOR_SIZE * VECTOR_SIZE];

    for (int a = 0; a < VECTOR_SIZE; a++)
        for (int b = 0; b < VECTOR_SIZE; b++)
        {
            covariance1[(a * VECTOR_SIZE) + b] = 0;
            covariance2[(a * VECTOR_SIZE) + b] = 0;
        }

    calculateCovariance(in, covariance1, 0, cutPoint, displacement);
    calculateCovariance(in, covariance2, cutPoint, sampleSize, displacement);

    float val = 0;

     for (int a = 0; a < VECTOR_SIZE; a++)
        for (int b = 0; b < VECTOR_SIZE; b++)
        {

            float t =  (covariance1[(VECTOR_SIZE * a) + b] - covariance2[(a * VECTOR_SIZE) + b]);
            val +=  t * t;
        }
     return val;

}
