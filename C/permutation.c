#include "permutation.h"
#include "utils.h"
#include <stdlib.h>

Permutation permutationCreate(int lenght)
{
	Permutation p;
	p.lenght = lenght;
	p.indicies = (unsigned*) malloc (sizeof(unsigned)*lenght);

	for (int a = 0; a < lenght; a++)
		p.indicies[a] = a;

	return p;
}

void permutationDestory(Permutation permutation)
{
	free (permutation.indicies);
}	


void permutationRandomize(Permutation permutation)
{
	int target;
	unsigned supp;
    for (int a = 0; a < permutation.lenght-2; a++)
	{
        target = randomNumber(a, permutation.lenght - 1);
		supp = permutation.indicies[target];
		permutation.indicies[target] = permutation.indicies[a];
		permutation.indicies[a] = supp;	
	}	
}
