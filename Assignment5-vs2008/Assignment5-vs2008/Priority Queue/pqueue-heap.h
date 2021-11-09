/**********************************************
 * File: pqueue-heap.h
 *
 * A priority queue class backed by a binary
 * heap.
 */
#ifndef PQueue_Heap_Included
#define PQueue_Heap_Included

#include <string>
using namespace std;

/* A class representing a priority queue backed by an
 * binary heap.
 */
class HeapPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a binary heap. */
	HeapPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~HeapPriorityQueue();
	
	/* Returns the number of elements in the priority queue. */
	int size();
	
	/* Returns whether or not the priority queue is empty. */
	bool isEmpty();
	
	/* Enqueues a new string into the priority queue. */
	void enqueue(string value);
	
	/* Returns, but does not remove, the lexicographically first string in the
	 * priority queue.
	 */
	string peek();
	
	/* Returns and removes the lexicographically first string in the
	 * priority queue.
	 */
	string dequeueMin();

private:

	string* heapQueue;
	int logicalLength;
	int capacity;

	/* Method keeps queue sorted after enqueueing or dequueuing element. 
	 * use bubble-up after enqueueing and bubble-down after dequeueing.
	 * n is index of wrong element in queue.
	 */
	void keepQueueSorted(int n, bool bubbleUp);

	/* Returns true if child is greater than parent.
	 * Otherwise swaps & returns false
	 */
	bool validProgeny(int child, int parent);

	/* Method increases size of HeapQueue. */
	void grow();
	
};

#endif
