/*
In this assignment you will implement one or more algorithms for the all-pairs shortest-path problem. Here are data files describing three graphs: graph #1; graph #2; graph #3.
The first line indicates the number of vertices and edges, respectively. Each subsequent line describes an edge (the first two numbers are its tail and head, respectively) and its length (the third number).
NOTE: some of the edge lengths are negative.
NOTE: These graphs may or may not have negative-cost cycles.

Your task is to compute the "shortest shortest path". Precisely, you must first identify which, if any, of the three graphs have no negative cycles.
For each such graph, you should compute all-pairs shortest paths and remember the smallest one (i.e., compute min d(u,v) (u,v¡ÊV), where d(u,v) denotes the shortest-path distance from u to v).

If each of the three graphs has a negative-cost cycle, then enter "NULL" in the box below.
If exactly one graph has no negative-cost cycles, then enter the length of its shortest shortest path in the box below.
If two or more of the graphs have no negative-cost cycles, then enter the smallest of the lengths of their shortest shortest paths in the box below.

OPTIONAL: You can use whatever algorithm you like to solve this question. If you have extra time, try comparing the performance of different all-pairs shortest-path algorithms!
OPTIONAL: If you want a bigger data set to play with, try computing the shortest shortest path for this graph "large.txt"
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "MinHeap.h"

using namespace std;

#define INFINITY 9999999
#define NEGATIVE_COST_CYCLE 9999999

struct VERTEX;
struct EDGE;

bool BellmanFordShortestPath(vector<VERTEX>&, int, int**);
void DijkstraShortestPath(vector<VERTEX>&, int, int*);

struct VERTEX
{
	int label;
	int penalty;
	EDGE* edgeIn;
	EDGE* edgeOut;
};

struct EDGE
{
	long distance;
	VERTEX* vertex;
	EDGE* next;
};

struct HeapElem
{
	int label;
	long distance;

	HeapElem(int newLabel, long newDistance) : label(newLabel), distance(newDistance) {};

	bool operator < (HeapElem other)
	{
		return distance < other.distance;
	}

	bool operator <= (HeapElem other)
	{
		return distance <= other.distance;
	}

	bool operator > (HeapElem other)
	{
		return distance > other.distance;
	}

	bool operator >= (HeapElem other)
	{
		return distance >= other.distance;
	}
};

int main()
{
	int results[3];
	string filenames[3] = {"g1.txt", "g2.txt", "g3.txt"};
	for(int i = 0; i < 3; ++i)
	{
		ifstream fin(filenames[i]);
		if(!fin)
		{
			cerr << "Error: unable to open "  << filenames[i].c_str() << endl;
			return -1;
		}

		int numberOfVertices, numberOfEdges;
		fin >> numberOfVertices >> numberOfEdges;

		vector<VERTEX> graph(numberOfVertices + 1);
		for(int j = 0; j < graph.size(); ++j)
		{
			graph.at(j).label = j;
			graph.at(j).penalty = 0;
			graph.at(j).edgeIn = NULL;
			graph.at(j).edgeOut = NULL;
		}
		for(int j = 1; j < graph.size(); ++j)
		{
			EDGE* edge1 = new EDGE;
			edge1->vertex = &graph.at(j);
			edge1->distance = 0;
			edge1->next = graph.at(0).edgeOut;
			graph.at(0).edgeOut = edge1;

			EDGE* edge2 = new EDGE;
			edge2->vertex = &graph.at(0);
			edge2->distance = 0;
			edge2->next = graph.at(j).edgeIn;
			graph.at(j).edgeIn = edge2;
		}

		for(int j = 0; j < numberOfEdges; ++j)
		{
			int label, adjLabel, distance;
			fin >> label >> adjLabel >> distance;

			EDGE* edge1 = new EDGE;
			edge1->vertex = &graph.at(adjLabel);
			edge1->distance = distance;
			edge1->next = graph.at(label).edgeOut;
			graph.at(label).edgeOut = edge1;

			EDGE* edge2 = new EDGE;
			edge2->vertex = &graph.at(label);
			edge2->distance = distance;
			edge2->next = graph.at(adjLabel).edgeIn;
			graph.at(adjLabel).edgeIn = edge2;
		}
		fin.close();

		int* A[2];
		A[0] = (int*)malloc((numberOfVertices + 1) * sizeof(int));
		A[1] = (int*)malloc((numberOfVertices + 1) * sizeof(int));
		if(BellmanFordShortestPath(graph, 0, A))
		{
			for(int j = 0; j < graph.size(); ++j)
				graph.at(j).penalty = A[0][j];

			free(A[0]);
			free(A[1]);

			int** B;
			B = (int**)malloc((numberOfVertices + 1) * sizeof(int*));
			for(int j = 1; j < graph.size(); ++j)
			{
				B[j] = (int*)malloc((numberOfVertices + 1) * sizeof(int));
				for(int k = 1; k < graph.size(); ++k)
					B[j][k] = INFINITY;
				DijkstraShortestPath(graph, j, B[j]);
			}

			results[i] = B[1][2];
			for(int j = 1; j < graph.size(); ++j)
			{
				for(int k = 1; k < graph.size(); ++k)
				{
					if(j != k && results[i] > B[j][k])
						results[i] = B[j][k];
				}
			}

			for(int j = 1; j < graph.size(); ++j)
				free(B[j]);
			free(B);
		}
		else
		{
			free(A[0]);
			free(A[1]);
			results[i] = NEGATIVE_COST_CYCLE;	
		}
	}

	int min;
	if(results[0] < results[1])
	{
		if(results[0] < results[2])
			min = results[0];
		else if(results[1] < results[2])
			min = results[1];
		else
			min = results[2];
	}
	else
	{
		if(results[1] < results[2])
			min = results[1];
		else
			min = results[2];
	}

	if(min == NEGATIVE_COST_CYCLE)
		cout << "NULL" << endl;
	else
		cout << min << endl;

	return 0;
}

bool BellmanFordShortestPath(vector<VERTEX>& graph, int srcLabel, int** A)
{
	for(int i = 0; i < graph.size(); ++i)
		A[0][i] = INFINITY;
	A[0][srcLabel] = 0;

	for(int j = 1; j <= graph.size(); ++j)
	{
		for(int i = 0; i < graph.size(); ++i)
		{
			A[j%2][i] = A[(j-1)%2][i];
			for(EDGE* node = graph.at(i).edgeIn; node; node = node->next)
			{
				int distance = A[(j-1)%2][node->vertex->label] + node->distance;
				if(distance <  A[j%2][i])
					A[j%2][i] = distance;
			}
		}
	}

	// check whether there exists a negative-cost cycle
	for(int i = 0; i < graph.size(); ++i)
		if(A[0][i] != A[1][i])
			return false;
	
	return true;
}

void DijkstraShortestPath(vector<VERTEX>& graph, int srcLabel, int* B)
{
	B[srcLabel] = 0;
	MinHeap<HeapElem> heap;
	for(EDGE* node = graph.at(srcLabel).edgeOut; node; node = node->next)
	{
		HeapElem* elem = new HeapElem(node->vertex->label, node->distance + graph.at(srcLabel).penalty - node->vertex->penalty);
		heap.insert(*elem);
	}

	while(heap.size())
	{
		HeapElem minElem = heap.extract_min();
		if(B[minElem.label] == INFINITY)
		{
			B[minElem.label] = minElem.distance - graph.at(srcLabel).penalty + graph.at(minElem.label).penalty;
			for(EDGE* node = graph.at(minElem.label).edgeOut; node; node = node->next)
			{
				HeapElem* elem = new HeapElem(node->vertex->label, minElem.distance + node->distance + graph.at(minElem.label).penalty - node->vertex->penalty);
				heap.insert(*elem);
			}
		}
	}

	return;
}