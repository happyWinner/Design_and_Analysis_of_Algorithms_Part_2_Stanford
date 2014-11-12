#include <malloc.h>
#include "WeightedQuickUnion.h"

// Create an empty union find data structure with N isolated sets
WeightedQuickUnion::WeightedQuickUnion(int N)
{
	counter = N;
	id = (int*)malloc(N * sizeof(int));
	size = (int*)malloc(N * sizeof(int));
	for(int i = 0; i < N; ++i)
	{
		id[i] = i;
		size[i] = 1;
	}
}

WeightedQuickUnion::~WeightedQuickUnion()
{
	free(id);
	free(size);
}

// Return the number of disjoint sets
int WeightedQuickUnion::count()
{
	return counter;
}

// Return component identifier for component containing p
int WeightedQuickUnion::find(int p)
{
	while(p != id[p])
		p = id[p];
	return p;
}

// Are objects p and q in the same set?
bool WeightedQuickUnion::connected(int p, int q)
{
	return find(p) == find(q);
}

// Replace sets containing p and q with their union
void WeightedQuickUnion::merge(int p, int q)
{
	int i = find(p);
	int j = find(q);
	if(i != j)
	{
		if(size[i] < size[j])
		{
			id[i] = j;
			size[j] += size[i];
		}
		else
		{
			id[j] = i;
			size[i] += size[j];
		}
		--counter;
	}
}