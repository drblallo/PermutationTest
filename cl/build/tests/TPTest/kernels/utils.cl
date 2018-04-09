//4294967291

unsigned int permutateQPR(unsigned int x, unsigned int prime)
{
	unsigned int residue = (((unsigned long)x) * ((unsigned long)x)) % prime;
	
	unsigned int t = (int)(x <= prime / 2);

	unsigned int returnValue = t * residue;

	t = 1 - t;
	returnValue += (prime - residue)* t; 
	return returnValue;
}

unsigned int next(unsigned int* seed, unsigned int intermediateOffset, unsigned prime)
{
	unsigned int t = permutateQPR(((permutateQPR(*seed, prime) + intermediateOffset  + 0x682f0161) % prime), prime);
	*seed = (*seed + 1) % prime;
	
	return t; //to change
}