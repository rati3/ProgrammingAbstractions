#include "pqueue-heap.h"
#include "error.h"

/* Constant controlling the default capacity of our queue. */
const int defaultSize = 8;


HeapPriorityQueue::HeapPriorityQueue() {
	logicalLength = 0;
	capacity = defaultSize;
	heapQueue = new string[capacity];	
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete[] heapQueue;
}

int HeapPriorityQueue::size() {
	return logicalLength;
}

bool HeapPriorityQueue::isEmpty() {
	return logicalLength == 0;
}

void HeapPriorityQueue::enqueue(string value) {
	if (logicalLength + 1 == capacity) grow(); // if no more free space
	heapQueue[++logicalLength] = value;       //  push back 
	keepQueueSorted(logicalLength, true);
}

bool HeapPriorityQueue::validProgeny (int child, int parent) {
	if (child > logicalLength || heapQueue[parent] < heapQueue[child])
		return true;
	swap(heapQueue[parent], heapQueue[child]);
	return false;
}

void HeapPriorityQueue::keepQueueSorted(int current, bool bubbleUp) {
	/* Base Case : if bubble-upping and it's top or bubble-downing and it's bottom */
	if ((bubbleUp && current == 1 ) || (!bubbleUp && 2 * current > logicalLength)) return;
	/* if bubble-upping opposite is parent, otherwise child */
	int opposite = int (current * ((bubbleUp) ? 0.5 : 2));
	/* if child is not greater than parent - check (2*n) and n */
	if((bubbleUp && !validProgeny(current, opposite)) || (!bubbleUp && !validProgeny(opposite,current))) keepQueueSorted(opposite, bubbleUp);
	/* if bubble-downing check (2*n + 1) and n */
	if (!bubbleUp && !validProgeny(opposite + 1, current)) keepQueueSorted(opposite + 1,bubbleUp);

}

string HeapPriorityQueue::peek() {
	if (isEmpty()) error("Queue is empty!");
	return heapQueue[1];
}

string HeapPriorityQueue::dequeueMin() {
	if (isEmpty()) error("queue is empty!");	
	string res = heapQueue[1];
	swap(heapQueue[1], heapQueue[logicalLength--]);
	keepQueueSorted(1,false);
	
	return res;
}

void HeapPriorityQueue::grow() {
	capacity = capacity * 2 + 1;
	string *tmp = new string[capacity];
	for (int i = 1; i <= logicalLength; i++) tmp[i] = heapQueue[i];
	delete[] heapQueue;
	heapQueue = tmp;
}

