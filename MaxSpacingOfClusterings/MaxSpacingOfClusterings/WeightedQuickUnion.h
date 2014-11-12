class WeightedQuickUnion
{
private:
	int counter;   // number of components
	int* id;     // id[i] = parent of i
	int* size;   // size[i] = number of objects in subtree rooted at i

public:
	WeightedQuickUnion(int N);      // Create an empty union find data structure with N isolated sets
	~WeightedQuickUnion();
	int count();                    // Return the number of disjoint sets
	int find(int p);                // Return component identifier for component containing p
	bool connected(int p, int q);   // Are objects p and q in the same set?
	void merge(int p, int q);       // Replace sets containing p and q with their union
};