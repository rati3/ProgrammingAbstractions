/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	head = new Node;
	head -> next = NULL;
	length = 0;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	Node * curr = head;
	while (curr != NULL) {
		Node* tmp = curr;
		curr = curr -> next;
		delete tmp;
	}

}

int LinkedListPriorityQueue::size() {
	return length;
}

bool LinkedListPriorityQueue::isEmpty() {
	return head -> next == NULL;
}

void LinkedListPriorityQueue::enqueue(string value) {
	Node* prev = getPrevNodeOf(value);
	Node* newNode = new Node;
	newNode -> value = value;
	newNode -> next = prev -> next; // insert node next to prev
	prev -> next = newNode;
	length++;
}

string LinkedListPriorityQueue::peek() {
	if (isEmpty()) error("Queue is empty!");
		
	return head -> next -> value;
}

string LinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) error("Queue is empty!");
		
	Node *tmp = head -> next;
	head -> next = tmp -> next;
	string res = tmp -> value;
	delete tmp;
	length--;
	return res;
}



LinkedListPriorityQueue::Node* LinkedListPriorityQueue::getPrevNodeOf(string val) {
	Node* curr = head;
	Node* nextNode;
	while ((nextNode = curr -> next) != NULL) {
		if (val < nextNode -> value) // if node is greater that val
			return curr;            // return prev node
		curr = nextNode;    
	}
	/* if there is no greater return last node */
	return curr;
}

