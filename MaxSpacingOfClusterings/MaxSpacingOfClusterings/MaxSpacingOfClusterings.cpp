/*
In this programming problem you'll code up the clustering algorithm from lecture for computing a max-spacing k-clustering.
This file describes a distance function (equivalently, a complete graph with edge costs). It has the following format:
[number_of_nodes]
[edge 1 node 1] [edge 1 node 2] [edge 1 cost]
[edge 2 node 1] [edge 2 node 2] [edge 2 cost]
...
There is one edge (i,j) for each choice of 1¡Üi<j¡Ün, where n is the number of nodes.
For example, the third line of the file is "1 3 5250", indicating that the distance between nodes 1 and 3 (equivalently, the cost of the edge (1,3)) is 5250.
You can assume that distances are positive, but you should NOT assume that they are distinct.

Your task in this problem is to run the clustering algorithm from lecture on this data set, where the target number k of clusters is set to 4. What is the maximum spacing of a 4-clustering?
*/

#include <iostream>
#include <fstream>
#include "MergeSort.h"
#include "WeightedQuickUnion.h"

// change the stack reserve size
#pragma comment(linker, "/STACK:102400000")

using namespace std;

#define NUMBER_OF_EDGES 124750
#define NUMBER_OF_CLUSTERINGS 4

struct EDGE
{
	int node1;
	int node2;
	int cost;

	bool operator <= (EDGE other)
	{
		return cost <= other.cost;
	}
};

int MaxSpacingOfClusterings(EDGE*, WeightedQuickUnion&, int, int);

int main()
{
	ifstream fin("clustering.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	int numberOfNodes;
	fin >> numberOfNodes;
	WeightedQuickUnion nodes = WeightedQuickUnion(numberOfNodes + 1);

	EDGE edges[NUMBER_OF_EDGES];
	int idx = 0;
	while(idx < NUMBER_OF_EDGES)
	{
		int node1, node2, cost;
		fin >> node1 >> node2 >> cost;
		edges[idx].node1 = node1;
		edges[idx].node2 = node2;
		edges[idx].cost = cost;
		++idx;
	}
	MergeSort<EDGE>(edges, 0, NUMBER_OF_EDGES - 1);

	cout << "Maximum Spacing of a " << NUMBER_OF_CLUSTERINGS << "-clustering: " << MaxSpacingOfClusterings(edges, nodes, numberOfNodes, NUMBER_OF_CLUSTERINGS) << endl;

	return 0;
}

int MaxSpacingOfClusterings(EDGE* edges, WeightedQuickUnion& nodes, int numberOfNodes, int numberOfClusterings)
{
	int idx = 0;
	while(nodes.count() - 1 != numberOfClusterings)
	{
		nodes.merge(edges[idx].node1, edges[idx].node2);
		++idx;
	}
	
	while(nodes.connected(edges[idx].node1, edges[idx].node2))
		++idx;

	return edges[idx].cost;
}