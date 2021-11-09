/* File: RandomSubset.cpp
 *
 * A solution to the Random Subset warmup problem for Assignment 3.
 */
#include <iostream>
#include "random.h"
#include "set.h"
#include "simpio.h"
using namespace std;

/* Function to choose a random subset of a set. */
Set<int> randomSubsetOf(Set<int>& s) {
	/* Base case: The only subset of the empty set is the empty set. */
	if (s.isEmpty()) return s;
	
	/* Extract the first element from the set. */
	int elem = s.first();
	
	/* Choose a random subset of what remains.  To do this, we will remove
	 * the first element, compute the random subset, then put the element
	 * back in.
	 */
	s -= elem;
	Set<int> result = randomSubsetOf(s);
	s += elem;
	
	/* Randomly choose whether to add the element back in. */
	return randomChance(0.5)? result : result + elem;
}

int main() {
	Set<int> testSet;
	testSet += 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;
	cout << randomSubsetOf(testSet) << endl;
}

