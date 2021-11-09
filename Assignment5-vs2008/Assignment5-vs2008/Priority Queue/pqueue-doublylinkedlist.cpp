#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	head = new Node;
	tail = new Node;
	head -> next = tail;
	tail -> prev = head;
	head -> prev = tail -> next = NULL;
	length = 0;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	Node* curr = head;
	while(curr != NULL) {
		Node *tmp = curr;
		curr = curr -> next;
		delete tmp;
	}
}

int DoublyLinkedListPriorityQueue::size() {
	return length;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {	
	return size() == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	Node *newNode = new Node;
	newNode -> value = value;
	newNode -> next = head -> next; // add at begin
	newNode -> prev = head;
	head -> next = newNode;
	newNode -> next -> prev = newNode;
	length++;
}

string DoublyLinkedListPriorityQueue::peek() {
	if(isEmpty()) error("Queue is empty!");
	Node* min = getMinNode();
	return min -> value;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	if(isEmpty()) error("Queue is empty!");
	Node* min = getMinNode();
	min -> next -> prev = min -> prev;
	min -> prev -> next = min -> next;
	string res = min -> value;
	delete min;
	length--;
	return res;
}

DoublyLinkedListPriorityQueue::Node* DoublyLinkedListPriorityQueue::getMinNode() {
	Node* curr = head;
	Node* min = curr -> next;
	while ((curr = curr -> next) != tail) 
		if (curr -> value < min -> value) min = curr;
	
	return min;
}	


