#include <iostream>
#include "console.h"
using namespace std;

int combinations(int n,int k);
void getNumbers(int &n, int &k);


int main() {
	int n,k;
	getNumbers(n,k);
	/* display combinations */
    cout << combinations(n,k) << endl;
    return 0;
}

int combinations(int n, int k) {
	if (n == k || k == 0) // if its edge of paskal
		return 1;
	return (combinations(n-1,k) + combinations(n-1,k-1));
}

/* get numbers from user */
void getNumbers(int &n, int &k){
	while (true) {
		cout << "n: ";
		cin >> n;
		cout << "k: ";
		cin >>k;
		if (n >= k && k >= 0) // if there are valid nums
			break;
	}
}

