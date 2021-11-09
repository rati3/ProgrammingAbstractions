#include "pqueue-extra.h"
#include "error.h"
#include <cmath>

const int defaultLength = 4;

ExtraPriorityQueue::ExtraPriorityQueue() {
	heap = Vector<Node *>(defaultLength, NULL);
}

ExtraPriorityQueue::~ExtraPriorityQueue() {
	for (int i = 0; i < heap.size(); i++)
		if (heap[i] != NULL) deleteNode(heap[i]);	
}

/* Node index of k, saves pow(2,k) element at binomial heap */
int ExtraPriorityQueue::size() {
	int length = 0;
	for (int i = 0; i < heap.size(); i++)
		if (heap[i] != NULL) length += (int) pow(2.0,(double)i);
	return length;
}

bool ExtraPriorityQueue::isEmpty() {
	return size() == 0;
}

ExtraPriorityQueue::Node* ExtraPriorityQueue::getNewNode(string val) {
	Node * newNode = new Node;
	newNode -> value = val;
	newNode -> order = 0;
	newNode -> firstChild = newNode -> sibling = NULL;

	return newNode;
}

void ExtraPriorityQueue::enqueue(string value) {
	Node * newNode = getNewNode(value);
	addNode(newNode);
}

string ExtraPriorityQueue::peek() {
	if (isEmpty()) error("queue is empty!");
	return getMinNode() -> value;
}

void ExtraPriorityQueue::bubbleUp(Node* child) {
	/* Base Case - No more sibiling */
	if (child == NULL) return;
	
	bubbleUp(child -> sibling);
	child -> sibling = NULL;
	addNode(child);	

}

string ExtraPriorityQueue::dequeueMin() {
	if (isEmpty()) error("queue is empty!");
	Node* minNode = getMinNode();          // find min Node in vector
	string res = minNode -> value;        // save its value
	heap[minNode -> order] = NULL;       // free space
	Node* currChild = minNode -> firstChild;
	bubbleUp(currChild);               // bubble up children of minNode

	
	delete minNode; 
	return res;
}

void ExtraPriorityQueue::addNode(Node * newNode) {
	int order = newNode -> order;

	if (order >= heap.size()) {
		growCapacity();
	} else if (heap[order] != NULL) {           // if node of order already exists at vector
		newNode = merge(newNode, heap[order]); // merge old and new 
	} else { // Base CASE - if enough size and NULL at order
		heap[order] = newNode;
		return;
	}
	/* Do Recursion */
	addNode(newNode);
}

void ExtraPriorityQueue::growCapacity() {
	Vector<Node *> temp(heap.size() * 2, NULL);
	for (int i = 0; i < heap.size(); i++) temp[i] = heap[i];
	heap = temp;
}


ExtraPriorityQueue::Node *ExtraPriorityQueue::merge(Node* &min, Node* &max) {
	/* max value must be greater than min */
	if (min -> value > max -> value) swap(min, max);

	max -> sibling = min -> firstChild;
	min -> firstChild = max;
	min -> order++;

	heap[max -> order] = NULL; // free space 
	return min;
}

ExtraPriorityQueue::Node* ExtraPriorityQueue::getMinNode() {
	int minIndex = -1;
	for (int i = 0; i < heap.size(); i++) 
		/* non-NULL and (first check or less than current min */
		if (heap[i] != NULL && (minIndex == -1 || heap[i] -> value < heap[minIndex] -> value)) minIndex = i;

	return heap[minIndex];
}

void ExtraPriorityQueue::deleteNode(Node* root) {
	Node* curr = root;
	while(curr != NULL) {
		deleteNode(curr -> firstChild);
		curr = curr -> sibling;
	}
	delete root;
}
