/*
In this programming problem, you'll code up Prim's minimum spanning tree algorithm.
This file describes an undirected graph with integer edge costs. It has the format
[number_of_nodes] [number_of_edges]
[one_node_of_edge_1] [other_node_of_edge_1] [edge_1_cost]
[one_node_of_edge_2] [other_node_of_edge_2] [edge_2_cost]
...
For example, the third line of the file is "2 3 -8874", indicating that there is an edge connecting vertex #2 and vertex #3 that has cost -8874.
You should NOT assume that edge costs are positive, nor should you assume that they are distinct.

Your task is to run Prim's minimum spanning tree algorithm on this graph. You should report the overall cost of a minimum spanning tree --- an integer, which may or may not be negative.

IMPLEMENTATION NOTES: This graph is small enough that the straightforward O(mn) time implementation of Prim's algorithm should work fine.

OPTIONAL: For those of you seeking an additional challenge, try implementing a heap-based version.
The simpler approach, which should already give you a healthy speed-up, is to maintain relevant edges in a heap (with keys = edge costs).
The superior approach stores the unprocessed vertices in the heap, as described in lecture. Note this requires a heap that supports deletions, and you'll probably need to maintain some kind of mapping between vertices and their positions in the heap.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "MinHeap.h"

using namespace std;

struct VERTEX;
struct EDGE;

long PrimMST(vector<VERTEX>&);

struct VERTEX
{
	int label;
	bool bExplored;
	EDGE* edge;
};

struct EDGE
{
	int label;
	int cost;
	EDGE* next;
};

struct HeapElem
{
	int label;
	int edgeCost;

	HeapElem(int newLabel, int newEdgeCost) : label(newLabel), edgeCost(newEdgeCost) {};

	bool operator < (HeapElem other)
	{
		return edgeCost < other.edgeCost;
	}

	bool operator <= (HeapElem other)
	{
		return edgeCost <= other.edgeCost;
	}

	bool operator > (HeapElem other)
	{
		return edgeCost > other.edgeCost;
	}

	bool operator >= (HeapElem other)
	{
		return edgeCost >= other.edgeCost;
	}
};

int main()
{
	ifstream fin("edges.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	int numberOfNodes, numberOfEdges;
	fin >> numberOfNodes >> numberOfEdges;

	vector<VERTEX> graph(numberOfNodes);
	for(int i = 0; i < numberOfNodes; ++i)
	{
		graph.at(i).label = i + 1;
		graph.at(i).bExplored = false;
		graph.at(i).edge = NULL;
	}

	for(int i = 0; i < numberOfEdges; ++i)
	{
		int label1, label2;
		int edgeCost;
		fin >> label1 >> label2 >> edgeCost;

		EDGE* edge1 = new EDGE;
		edge1->label = label2;
		edge1->cost = edgeCost;
		edge1->next = graph.at(label1 - 1).edge;
		graph.at(label1 - 1).edge = edge1;

		EDGE* edge2 = new EDGE;
		edge2->label = label1;
		edge2->cost = edgeCost;
		edge2->next = graph.at(label2 - 1).edge;
		graph.at(label2 - 1).edge = edge2;
	}

	cout << "Overall Cost of the Minimum Spanning Tree: " << PrimMST(graph) << endl;

	return 0;
}

long PrimMST(vector<VERTEX>& graph)
{
	long overallCost = 0L;
	graph.at(0).bExplored = true;
	MinHeap<HeapElem> heap;
	for(EDGE* node = graph.at(0).edge; node; node = node->next)
	{
		HeapElem* elem = new HeapElem(node->label, node->cost);
		heap.insert(*elem);
		delete elem;
	}

	while(heap.size())
	{
		HeapElem elem = heap.extract_min();
		int index = elem.label - 1;
		if(!graph.at(index).bExplored)
		{
			overallCost += elem.edgeCost;
			graph.at(index).bExplored = true;
			for(EDGE* node = graph.at(index).edge; node; node = node->next)
			{
				if(!graph.at(node->label - 1).bExplored)
				{
					HeapElem* elem = new HeapElem(node->label, node->cost);
					heap.insert(*elem);
					delete elem;
				}
			}
		}
	}

	return overallCost;
}