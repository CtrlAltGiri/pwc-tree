#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<limits.h>
#include<time.h>

#define RIGHT next.right
#define REM next.rem
#define CHILD child
#define MAX_LIMIT 700    // defines the maximum number of products in a transaction.
#define fileName "Tumor/tumor14.csv"
#define primeFile "Includes/primes.csv"
#define limitPrime 20000
#define INT_MAX_FINAL 9007199254740991

int *allPrimes, oldValueI = 0, threshold;
int tempMem = 0;

union link{
	struct node* right;		//indicates not remaining node
	struct rem_Node* rem;		//indicate remaining value
};

typedef struct rem_Node{
	unsigned long int val;
	char flag;
	union link next;
}*REMNODE;

typedef struct node{
	unsigned long int val;
	unsigned int qty;
	char flag;			//r for remaining nodes
	union link next;
	struct node* child;
}*NODE;
