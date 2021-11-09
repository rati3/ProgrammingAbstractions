/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
}

VectorPriorityQueue::~VectorPriorityQueue() {
}

int VectorPriorityQueue::size() {
	return vectorQueue.size();
}

bool VectorPriorityQueue::isEmpty() {
	return size() == 0;
}

void VectorPriorityQueue::enqueue(string value) {	
	vectorQueue.add(value);
}

string VectorPriorityQueue::peek() {
	if (isEmpty()) error("queue is empty!");
		
	return vectorQueue[lexicoFirst()];
}

string VectorPriorityQueue::dequeueMin() {
	if (isEmpty()) error("queue is empty!");
		
	string temp = vectorQueue[lexicoFirst()];
	vectorQueue.remove(lexicoFirst());
	return temp;
}


int VectorPriorityQueue::lexicoFirst() {
	int minIndex = 0;
	for (int i = 1; i < vectorQueue.size(); i++) 
		if (vectorQueue[i] < vectorQueue[minIndex]) minIndex = i;
			
	return minIndex;
}

