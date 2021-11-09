#ifndef PQueue_Extra_Included
#define PQueue_Extra_Included

#include <string>
#include "vector.h"
using namespace std;

/*
 * TODO: Describe this class!
 */
class ExtraPriorityQueue {
public:
	/* Constructs a new, empty priority queue. */
	ExtraPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~ExtraPriorityQueue();
	
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
	struct Node {
		string value;
		int order; // number of child
		Node* firstChild; 
		Node* sibling;
	};

	/* Main Data - roots of trees. At index of k is Node of order k or NULL */
	Vector<Node *> heap;

	/* adds Node at vector */
	void addNode(Node * newNode);

	/* get new Node with default values */
	Node* getNewNode(string val);

	/* if it is necessary increase capacity of vector */
	void growCapacity();

	/* Method merges two same order Node and returns it */
	Node* merge(Node* &node1, Node* &node2);

	/* Returns node with min value from heap(vecor) */ 
	Node* getMinNode();

	/* deletes children and sibilings of root and then root */
	void deleteNode(Node* root);

	/* Method adds child and its all sibiling at heap(vector) */
	void bubbleUp(Node* child);
	
};

#endif
