/*
In this assignment you will implement one or more algorithms for the 2SAT problem.
The file format is as follows. In each instance, the number of variables and the number of clauses is the same, and this number is specified on the first line of the file.
Each subsequent line specifies a clause via its two literals, with a number denoting the variable and a "-" sign denoting logical "not".
For example, the second line of the first data file is "-16808 75250", which indicates the clause !x16808 || x75250.

Your task is to determine which of the 6 instances are satisfiable, and which are unsatisfiable. 

DISCUSSION: This assignment is deliberately open-ended, and you can implement whichever 2SAT algorithm you want.
For example, 2SAT reduces to computing the strongly connected components of a suitable graph (with two vertices per variable and two directed edges per clause, you should think through the details).
Alternatively, you can use Papadimitriou's randomized local search algorithm. (The algorithm from lecture might be too slow, so you might want to make one or more simple modifications to it to ensure that it runs in a reasonable amount of time.)
A third approach is via backtracking. In lecture we mentioned this approach only in passing; see the DPV book, for example, for more details.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <time.h>

using namespace std;

const int numFiles = 6;
const string files[] = {"2sat1.txt", "2sat2.txt", "2sat3.txt", "2sat4.txt", "2sat5.txt", "2sat6.txt"};

struct CLAUSE
{
	long var1;
	long var2;

	CLAUSE(long theVar1, long theVar2) : var1(theVar1), var2(theVar2) {};

	void operator = (CLAUSE& that)
	{
		var1 = that.var1;
		var2 = that.var2;
	}
};

void DataReduction(vector<CLAUSE>&);
bool PapadimitriouTwoSAT(vector<CLAUSE>&);

int main()
{
	time_t start, end;
	time(&start);

	for(int i = 0; i < numFiles; ++i)
	{
		ifstream fin(files[i]);
		if(!fin)
			cerr << "Error: unable to open input file." << endl;

		long numClauses;
		fin >> numClauses;

		vector<CLAUSE> clauses;
		for(long j = 0; j < numClauses; ++j)
		{
			long variable1, variable2;
			fin >> variable1 >> variable2;
			clauses.push_back(CLAUSE(variable1, variable2));
		}
		fin.close();

		// get rid of unnecessary clauses and variables
		DataReduction(clauses);

		cout << files[i] << ": ";
		if(PapadimitriouTwoSAT(clauses))
			cout << "Satisfiable" << endl;
		else
			cout << "Unsatisfiable" << endl;
	}

	time(&end);
	cout << "Running Time: " << difftime(end, start) << " seconds" << endl;

	return 0;
}

void DataReduction(vector<CLAUSE>& clauses)
{
	map<long, bool> variables;
	unordered_set<long> redundancy;
	bool bContinue = true;
	while(bContinue)
	{
		bContinue = false;
		for(auto iter = clauses.begin(); iter != clauses.end(); ++iter)
		{
			for(int j = 0; j < 2; ++j)
			{
				long var;
				if(j)
					var = iter->var2;
				else
					var = iter->var1;
				auto it = variables.find(var);
				if(it == variables.end())
				{
					variables.insert(pair<long, bool>(var, true));
					variables.insert(pair<long, bool>(-var, false));
				}
				else
					it->second = true;
			}
		}
		
		for(auto it = variables.begin(), rit = variables.end(); it != variables.end() && it->first < 0; ++it)
		{
			--rit;
			long var = 0;
			if(it->second && !rit->second)
				redundancy.insert(it->first);
			if(!it->second && rit->second)
				redundancy.insert(rit->first);
		}
		variables.clear();

		if(!redundancy.empty())
		{
			bContinue = true;
			for(long i = 0; i < clauses.size();)
			{
				if(redundancy.find(clauses.at(i).var1) != redundancy.end()
					|| redundancy.find(clauses.at(i).var2) != redundancy.end())
				{
					clauses.at(i) = clauses.back();
					clauses.pop_back();
				}
				else
					++i;
			}
			redundancy.clear();
		}
	}
	return;
}

bool PapadimitriouTwoSAT(vector<CLAUSE>& clauses)
{
	if(clauses.empty())
		return true;

	map<long, bool> variables;
	for(long i = 0; i < clauses.size(); ++i)
	{
		for(int j = 0; j < 2; ++j)
		{
			long var = 0;
			if(j)
				var = abs(clauses.at(i).var1);
			else
				var = abs(clauses.at(i).var2);
			auto it = variables.find(var);
			if(it == variables.end())
				variables.insert(pair<long, bool>(var, true));
		}
	}

	long numOuterLoop = log(variables.size()) / log(2) + 1;
	long numInnerLoop = 2 * variables.size() * variables.size();
	for(long i = 0; i < numOuterLoop; ++i)
	{
		// choose random initial assignment
		srand((unsigned)time(NULL));
		for(auto it = variables.begin(); it != variables.end(); ++it)
			it->second = rand() % 2 == 0;

		for(long j = 0; j < numInnerLoop; ++j)
		{
			bool bSatisfiable = true;
			for(auto it = clauses.begin(); bSatisfiable && it != clauses.end(); ++it)
			{
				auto iter1 = variables.find(abs(it->var1));
				auto iter2 = variables.find(abs(it->var2));
				bool match1 = (it->var1 > 0 && iter1->second || it->var1 < 0 && !iter1->second);
				bool match2 = (it->var2 > 0 && iter2->second || it->var2 < 0 && !iter2->second);
				if(!match1 && !match2)
				{
					bSatisfiable = false;
					if(!(rand() % 2))
						iter1->second = !iter1->second;
					else
						iter2->second = !iter2->second;
				}
			}
			if(bSatisfiable)
				return true;
		}
	}

	return false;
}