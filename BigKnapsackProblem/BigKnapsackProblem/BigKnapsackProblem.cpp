/*
This problem asks you to solve a knapsack instance, but a much bigger one.
This file describes a knapsack instance, and it has the following format:
[knapsack_size][number_of_items]
[value_1] [weight_1]
[value_2] [weight_2]
...
For example, the third line of the file is "50074 834558", indicating that the second item has value 50074 and size 834558, respectively.
You should assume that item weights and the knapsack capacity are integers.

This instance is so big that the straightforward iterative implemetation uses an infeasible amount of time and space. So you will have to be creative to compute an optimal solution.
One idea is to go back to a recursive implementation, solving subproblems --- and, of course, caching the results to avoid redundant work --- only on an "as needed" basis.
Also, be sure to think about appropriate data structures for storing and looking up solutions to subproblems.
*/

#include <iostream>
#include <fstream>

using namespace std;

int main()
{
	ifstream fin("knapsack_big.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}
	
	int capacity, itemNumber;
	fin >> capacity >> itemNumber;
	int *values, *weights;
	values = (int*)malloc(itemNumber * sizeof(int));
	weights = (int*)malloc(itemNumber * sizeof(int));
	for(int i = 0; i < itemNumber; ++i)
		fin >> values[i] >> weights[i];

	int** optimalValue;
	optimalValue = (int**)malloc(2 * sizeof(int*));
	optimalValue[0] = (int*)malloc((capacity + 1) * sizeof(int));
	optimalValue[1] = (int*)malloc((capacity + 1) * sizeof(int));

	for(int i = 0; i < capacity + 1; ++i)
		optimalValue[0][i] = 0;

	for(int i = 1; i < itemNumber + 1; ++i)
		for(int j = 0; j < capacity + 1; ++j)
			if(j < weights[i - 1])
				optimalValue[i % 2][j] = optimalValue[(i - 1) % 2][j];
			else
				optimalValue[i % 2][j] = max(optimalValue[(i - 1) % 2][j], optimalValue[(i - 1) % 2][j - weights[i - 1]] + values[i - 1]);

	cout << max(optimalValue[0][capacity], optimalValue[1][capacity]) << endl;

	return 0;
}