#pragma once

typedef struct prm 
{
	unsigned* indicies;
	int lenght;	
} Permutation;

Permutation permutationCreate(int lenght);
void permutationDestory(Permutation permutation);
void permutationRandomize(Permutation permutation);
