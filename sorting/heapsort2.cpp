#include <iostream>
#include <algorithm>

using namespace std;

template<typename type>
void maxheapify(type* heap, unsigned int heap_size, unsigned int idx=0)
{
	--heap;
	++idx;
	unsigned int son, next=idx, prev;
	type tmp = heap[idx];
	while(prev = next, true)
	{
		son = next << 1;
		if(son <= heap_size && tmp < heap[son])
		{
			if(son + 1 <= heap_size && heap[son] < heap[son+1])
				++son;
		}
		else if(son + 1 <= heap_size && tmp < heap[son+1])
			++son;
		else
			break;
		heap[prev] = heap[next = son];
	}
	heap[next]=tmp;
}

template<typename type>
unsigned int build_maxheap(type* begin, type* end)
{
	unsigned int size=end-begin;
	for(int i=(size>>1); i>0;)
		maxheapify(begin, size, --i);
return size;
}

template<typename type>
void heapsort(type* begin, type* end)
{
	unsigned int heap_size=build_maxheap(begin, end);
	while(--heap_size>0)
	{
		swap(begin[0], begin[heap_size]);
		maxheapify(begin, heap_size);
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	int *tab;
	int n;
	cin >> n;
	tab=new int[n];
	for(int i=0; i<n; ++i)
		cin >> tab[i];
#ifdef QUICKSORT
	sort(tab, tab+n);
#else
	heapsort(tab, tab+n);
#endif
	cout << tab[0];
	for(int i=1; i<n; ++i)
		cout << ' ' << tab[i];
	cout << endl;
	delete[] tab;
return 0;
}