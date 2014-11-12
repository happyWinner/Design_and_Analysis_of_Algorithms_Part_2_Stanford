/*
In this assignment you will implement one or more algorithms for the traveling salesman problem, such as the dynamic programming algorithm covered in the video lectures.
Here is a data file describing a TSP instance. The first line indicates the number of cities. Each city is a point in the plane, and each subsequent line indicates the x- and y-coordinates of a single city.
The distance between two cities is defined as the Euclidean distance.

You should ouput the minimum cost of a traveling salesman tour for this instance.

HINT: You might experiment with ways to reduce the data set size. For example, trying plotting the points. Can you infer any structure of the optimal solution? Can you use that structure to speed up your algorithm?
*/

#include <iostream>
#include <fstream>

using std::ifstream;
using std::cout;
using std::cerr;
using std::endl;

const double INFINITY = 9999999999.9;

struct City
{
	double x;
	double y;

	City(double theX, double theY) : x(theX), y(theY) {};
};

double TravelingSalesmanProblem(City*, int);
double distance(City*, City*);
double getMin(double, double);
unsigned int combination(int, int);
unsigned int elems2idx(int, int*);
void idx2elems(int, int*, unsigned int);

int main()
{
	ifstream fin("tsp.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	int numCity;
	fin >> numCity;

	City* cities = (City*)malloc(numCity * sizeof(City));
	for(int i = 0; i < numCity; ++i)
	{
		double x, y;
		fin >> x >> y;
		cities[i] = *(new City(x, y));
	}

	cout << "Minimum Cost of A Traveling Salesman Tour: " << TravelingSalesmanProblem(cities, numCity) << endl;

	free(cities);
	return 0;
}

double TravelingSalesmanProblem(City* cities, int numCity)
{
	double*** A = (double***)malloc(2 * sizeof(double**));
	A[0] = (double**)malloc(numCity * sizeof(double*));
	A[1] = (double**)malloc(numCity * sizeof(double*));
	for(int i = 0; i < numCity; ++i)
	{
		A[0][i] = (double*)malloc(combination(numCity - 1, (numCity - 1) / 2) * sizeof(double));
		A[1][i] = (double*)malloc(combination(numCity - 1, (numCity - 1) / 2) * sizeof(double));
	}

	// base case
	int start = numCity - 1;
	for(unsigned int i = 0; i < combination(numCity - 1, (numCity - 1) / 2); ++ i)
	{
		A[0][start][i] = INFINITY;
		A[1][start][i] = INFINITY;
	}
	int startIdx[] = {start};
	A[0][start][elems2idx(1, startIdx)] = 0;

	for(int m = 1; m < numCity; ++m)     // m = subproblem size
	{
		for(unsigned int i = 0; i < combination(numCity - 1, m); ++i)
		{
			int* elems = (int*)malloc(m * sizeof(int));
			idx2elems(m, elems, i);
			for(int j = 0; j < m; ++j)
			{
				int elem = elems[j];
				int* remain;
				if(m != 1)
				{
					remain = (int*)malloc((m - 1) * sizeof(int));
					for(int idx1 = 0, idx2 = 0; idx2 < m; ++idx2)
					{
						if(idx2 != j)
							remain[idx1++] = elems[idx2];
					}
				}
				else
				{
					remain = (int*)malloc(sizeof(int));
					remain[0] = start;
				}

				int remainSize;
				if(m == 1)
					remainSize = 1;
				else
					remainSize = m - 1;
				double min = INFINITY;
				for(int k = 0; k < remainSize; ++k)
				{
					int dst = remain[k];
					double newDistance = A[(m+1)%2][dst][elems2idx(remainSize, remain)] + distance(&cities[dst], &cities[elem]);
					if(min > newDistance)
						min = newDistance;
				}
				A[m%2][elem][i] = min;

				free(remain);
			}
			free(elems);
		}
	}

	double min = INFINITY;
	for(int i = 0; i < numCity - 1; ++i)
	{
		double newDistance = A[(numCity-1)%2][i][0] + distance(&cities[i], &cities[start]);
		if(min > newDistance)
			min = newDistance;
	}
	
	free(A[1]);
	free(A[0]);
	free(A);

	return min;
}

double distance(City* a, City* b)
{
	return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

double getMin(double a, double b)
{
	return a < b ? a : b;
}

unsigned int combination(int n, int k)
{
	// apply recursion in order to prevent overflow
	// e.g. factorial(21) overflows without recursion
	if(n > 20)
	{
		if(n < k)
			return 0;
		else if(n == k)
			return 1;
		else if(k == 1)
			return n;
		else
			return combination(n - 1, k - 1) + combination(n - 1, k);
	}
	else
	{
		if(n <= 0 || k < 0 || n < k)
			return 0;
		else if (k == 0)
			return 1;
		else
		{
			unsigned long long numerator = 1;
			for(int i = n - k + 1; i <= n; ++i)
				numerator *= i;
			
			unsigned long long denominator = 1;
			for(int i = 2; i <= k; ++i)
				denominator *= i;
			
			return numerator / denominator;
		}
	}
}

unsigned int elems2idx(int numSet, int* elems)
{
	unsigned int idx = 0;
	for(int i = 0; i < numSet; ++i)
		idx += combination(elems[i], i + 1);
	return idx;
}

void idx2elems(int numSet, int* elems, unsigned int idx)
{
	while(numSet > 0)
	{
		int elem = numSet - 1;
		while(idx >= combination(elem, numSet))
			++elem;
		idx -= combination(--elem, numSet--);
		elems[numSet] = elem;
	}
}