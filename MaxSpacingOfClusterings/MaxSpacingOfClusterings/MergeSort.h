template<typename Comparable>
void MergeSort(Comparable* elems, int start, int end)
{
	int middle = (start + end) / 2;
	if(start != end)
	{
		MergeSort(elems, start, middle);
		MergeSort(elems, middle + 1, end);
	}

	int lIdx = start;
	int rIdx = middle + 1;
	int sortedIdx = start;
	Comparable* sortedElems = (Comparable*)malloc((end - start + 1) * sizeof(Comparable));
	for(; sortedIdx <= end && lIdx <= middle && rIdx <= end; ++sortedIdx)
	{
		if(elems[lIdx] <= elems[rIdx])
		{
			sortedElems[sortedIdx - start] = elems[lIdx];
			++lIdx;
		}
		else
		{
			sortedElems[sortedIdx - start] = elems[rIdx];
			++rIdx;
		}
	}

	while(lIdx <= middle)
	{
		sortedElems[sortedIdx - start] = elems[lIdx];
		++lIdx;
		++sortedIdx;
	}

	while(rIdx <= end)
	{
		sortedElems[sortedIdx - start] = elems[rIdx];
		++rIdx;
		++sortedIdx;
	}

	for(sortedIdx = start; sortedIdx <= end; ++sortedIdx)
		elems[sortedIdx] = sortedElems[sortedIdx - start];

	free(sortedElems);
}