/*
In this programming problem and the next you'll code up the greedy algorithms from lecture for minimizing the weighted sum of completion times.
This file describes a set of jobs with positive and integral weights and lengths. It has the format
[number_of_jobs]
[job_1_weight] [job_1_length]
[job_2_weight] [job_2_length]
...
For example, the third line of the file is "74 59", indicating that the second job has weight 74 and length 59.
You should NOT assume that edge weights or lengths are distinct.

Question 1:
Your task in this problem is to run the greedy algorithm that schedules jobs in decreasing order of the difference (weight - length). Recall from lecture that this algorithm is not always optimal.
IMPORTANT: if two jobs have equal difference (weight - length), you should schedule the job with higher weight first.
Beware: if you break ties in a different way, you are likely to get the wrong answer. You should report the sum of weighted completion times of the resulting schedule --- a positive integer.

Question 2:
For this problem, use the same data set as in the previous problem. Your task now is to run the greedy algorithm that schedules jobs (optimally) in decreasing order of the ratio (weight/length).
In this algorithm, it does not matter how you break ties. You should report the sum of weighted completion times of the resulting schedule --- a positive integer.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "MaxHeap.h"

using namespace std;

const double epsilon = 0.000001;
struct Job;

long long JobScheduling(vector<Job>&);

struct Job
{
	int weight;
	int length;
	double priority;

	Job(int weight, int length, double priority)
	{
		this->weight = weight;
		this->length = length;
		this->priority = priority;
	}

	bool operator < (const Job& other)
	{
		if(priority - other.priority <= epsilon && priority - other.priority >= 0
			|| other.priority - priority <= epsilon && other.priority - priority >= 0)
			return weight <= other.weight;
		else
			return priority < other.priority;
	}

	bool operator <= (const Job& other)
	{
		return *this < other;
	}

	bool operator > (const Job& other)
	{
		if(priority - other.priority <= epsilon && priority - other.priority >= 0
			|| other.priority - priority <= epsilon && other.priority - priority >= 0)
			return weight >= other.weight;
		else
			return priority > other.priority;
	}

	bool operator >= (const Job& other)
	{
		return *this > other;
	}
};

int main()
{
	ifstream fin("jobs.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	int numberOfJobs;
	fin >> numberOfJobs;

	vector<Job> jobsDifference;
	vector<Job> jobsRatio;
	for(int i = 0; i < numberOfJobs; ++i)
	{
		int weight, length;
		fin >> weight;
		fin >> length;

		Job* jobDifference = new Job(weight, length, (double) weight - length);
		jobsDifference.push_back(*jobDifference);
		delete jobDifference;

		Job* jobRatio = new Job(weight, length, (double) weight / length);
		jobsRatio.push_back(*jobRatio);
		delete jobRatio;
	}
	fin.close();

	cout << "Weighted Completion Time of the Schedule by Difference: " << JobScheduling(jobsDifference) << endl;
	cout << "Weighted Completion Time of the Schedule by Ratio: " << JobScheduling(jobsRatio) << endl;

	return 0;
}

long long JobScheduling(vector<Job>& jobs)
{
	long long currentTime = 0;
	long long weightedCompletionTime = 0;
	MaxHeap<Job> jobSchedule;
	jobSchedule.heapify(jobs);

	while(jobSchedule.size())
	{
		Job job = jobSchedule.extract_max();
		currentTime += job.length;
		weightedCompletionTime += job.weight * currentTime;
	}

	return weightedCompletionTime;
}