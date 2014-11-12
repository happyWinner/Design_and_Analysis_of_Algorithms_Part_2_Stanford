/*
In this programming problem ayou'll code up the knapsack algorithm from lecture.
This file describes a knapsack instance, and it has the following format:
[knapsack_size][number_of_items]
[value_1] [weight_1]
[value_2] [weight_2]
...
For example, the third line of the file is "50074 659", indicating that the second item has value 50074 and size 659, respectively.
You can assume that all numbers are positive. You should assume that item weights and the knapsack capacity are integers.
*/

#include <iostream>
#include <fstream>

using namespace std;

int max(int a, int b)
{
	return a > b ? a : b;
}

int main()
{
	ifstream fin("knapsack.txt");
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
	optimalValue = (int**)malloc((itemNumber + 1) * sizeof(int*));
	for(int i = 0; i < itemNumber + 1; ++i)
		optimalValue[i] = (int*)malloc((capacity + 1) * sizeof(int));

	for(int j = 0; j < capacity + 1; ++j)
		optimalValue[0][j] = 0;

	for(int i = 1; i < itemNumber + 1; ++i)
		for(int j = 0; j < capacity + 1; ++j)
			if(j < weights[i - 1])
				optimalValue[i][j] = optimalValue[i - 1][j];
			else
				optimalValue[i][j] = max(optimalValue[i - 1][j], optimalValue[i - 1][j - weights[i - 1]] + values[i - 1]);


	cout << "The value of the optimal solution: " << optimalValue[itemNumber][capacity] << endl;

	return 0;
}