#include "structures.h"

unsigned long int MemUsage = 0;

int isPrime(int x, int *mid){
	// binary search to check if x is a prime
	int beg = 0, end = limitPrime;

	while(beg <= end){
		*mid = (beg + end) / 2;
		if(allPrimes[*mid] == x)
			return 1;
		if(allPrimes[*mid] < x)
			beg = *mid + 1;
		else
			end = *mid - 1;
	}
	return 0;

}

void findPrimeFactors(unsigned long int finals, int *j, int *MulPrimes, int shouldContinue){

	int i;
	if(shouldContinue != 0)
		i = oldValueI;
	else
		i = 0;

	for(; i < limitPrime && finals > 1; i++){
		if(finals % (unsigned long int)allPrimes[i] == 0){
			finals /= allPrimes[i];
			MulPrimes[((*j)++)] = allPrimes[i];
		}
	}
	oldValueI = i;
}

NODE createNode(int* values, int count, int qty, NODE child, NODE right){

	NODE newNode = (NODE)malloc(sizeof(struct node));
	newNode -> qty = qty;
	newNode -> flag = 'n';
	newNode -> CHILD = child;

	int i = 0;
	unsigned long int temp = 1;
	for(; i < count; i++){
		if(INT_MAX_FINAL / temp > values[i]){
			temp *= values[i];
		}
		else{
			break;
		}
	}
	newNode -> val = temp;

	if(i == count){
		newNode -> RIGHT = right;
		return newNode;
	}
	newNode -> flag = 'r';

	REMNODE remNodeTemp = (REMNODE)malloc(sizeof(struct rem_Node));
	newNode -> REM = remNodeTemp;
	remNodeTemp -> flag = 'n';
	remNodeTemp -> RIGHT = right;

	temp = 1;
	for(; i < count; i++){
		if(INT_MAX_FINAL / temp > values[i])
			temp *= values[i];
		else{
			remNodeTemp -> val = temp;
			temp = 1;
			if(i + 1 != count){
				i--;
				remNodeTemp -> flag = 'r';
				REMNODE remNode = (REMNODE)malloc(sizeof(struct rem_Node));
				remNodeTemp -> REM = remNode;
				remNode -> flag = 'n';
				remNode -> RIGHT = right;
				remNodeTemp = remNode;
			}
		}
	}
	remNodeTemp -> val = temp;
	return newNode;
}

REMNODE findRightMost(NODE traverse){
	REMNODE veryTemp;
	if(traverse -> flag == 'r'){
		veryTemp = traverse -> REM;
		while(veryTemp -> flag == 'r')
			veryTemp = veryTemp -> REM;
	}
	return veryTemp;
}

void findFactorsNode(int *a, int *count, NODE traverse){

	findPrimeFactors(traverse -> val, count, a, 0);

	if(traverse -> flag == 'r'){
		REMNODE temp = traverse -> REM;
		while(temp -> flag == 'r'){
			findPrimeFactors(temp -> val, count, a, 1);
			temp = temp -> REM;
		}
		findPrimeFactors(temp -> val, count, a, 1);
	}
}

int countIntersection(int *arr1, int *arr2, int m, int n){
	int i = 0, j = 0, count = 0;
	while (i < m && j < n){
		if (arr1[i] < arr2[j])
	  		i++;

		else if (arr2[j] < arr1[i])
	  		j++;

		else{
			count++;
	  		j++;
	  		i++;
		}
	}
	return count;
}

int removeIntersection(int **arr1, int *arr2, int *x, int n, short extra){
	// the first array you pass will be replaced with a 1.
	int m = *x;
	int *temp = (int*)malloc(sizeof(int) * (*x));
	int k = 0;
	int i = 0, j = 0, count = 0;
	while (i < m && j < n){
		if ((*arr1)[i] < arr2[j]){
			temp[k++] = (*arr1)[i++];
		}
		else if (arr2[j] < (*arr1)[i]){
			j++;
		}
		else{
			count++;
	  		j++;
	  		i++;
		}
	}

	while(i < m){
		temp[k++] = (*arr1)[i++];
	}

	if(k != 0){
		temp = (int*)realloc(temp, k * sizeof(int));
		if(extra == 1){
			free(*arr1);
		}
		*arr1 = temp;
	}
	*x = k;

	return count;
}

int isDivisibleArrayNode(int *values, NODE traverse, int count, short extra){

	/*
		extra == 0, check both of the following:
		extra == 1, check if values is the bigger value.
		extra == 2, check if traverse is bigger.

	*/

	int case1, case2;

	int *a = (int*)malloc(sizeof(int) * MAX_LIMIT);
	int counta = 0;
	findFactorsNode(a,&counta,traverse);

	int x = countIntersection(a, values, counta, count);

	free(a);
	if(extra == 1){
		if(x == counta)
			return 1;
	}
	else if(extra == 2){
		if(x == count)
			return 1;
	}
	else{
		if(count > counta){
			if(x == counta){
				return 1;
			}
		}
		else if(counta > count){
			if(x == count)
				return 2;
		}
		else{
			if(counta == count && x == count){
				return -1;
			}
		}
	}
	return 0;
}

void removeFactorsArray(int **values, NODE *traverse, int *count, short extra){

	int *a = (int*)malloc(sizeof(int) * MAX_LIMIT);
	int counta = 0;
	findFactorsNode(a, &counta, *traverse);

	if(extra == 1){
		removeIntersection(values,a,count,counta, 0);
		free(a);
	}
	else{
		removeIntersection(&a,*values,&counta,*count, 1);
		NODE temp = *traverse;
		if(temp -> flag == 'r')
			*traverse = createNode(a, counta, temp -> qty, temp -> CHILD, findRightMost(temp) -> RIGHT);
		else
			*traverse = createNode(a, counta, temp -> qty, temp -> CHILD, temp-> RIGHT);
		free(a);
		free(temp);			// idk
	}
}

void removeCommonFactorsNodes(NODE *trav1, NODE trav2){

	// both are r. Remove NODE 1's factors which are common.
	int *a = (int*)malloc(sizeof(int) * MAX_LIMIT), *b = (int*)malloc(sizeof(int) * MAX_LIMIT);
	int count1 = 0, count2 = 0;

	findFactorsNode(a, &count1, *trav1);
	findFactorsNode(b, &count2, trav2);

	removeIntersection(&a,b,&count1, count2, 1);
	NODE temp = *trav1;
	if(temp -> flag == 'r')
		*trav1 = createNode(a, count1, temp -> qty, temp -> CHILD,  findRightMost(temp) -> RIGHT);
	else
		*trav1 = createNode(a, count1, temp -> qty, temp -> CHILD, temp-> RIGHT);

	free(a);
	free(b);

}

void moveNonFactorsRight(NODE newNode, NODE traverse, int *values, int count){

	NODE mainNode = newNode;
	REMNODE tTemp,aTemp;
	char whereToGo, nodeWhat;

	/*
	tTemp is the newNode's right and aTemp is the traverse's right.

	tTemp = findRightMost(traverse);

	if(traverse -> flag == 'n' && traverse -> RIGHT && traverse -> RIGHT -> flag == 'r')
		tTemp = findRightMost(traverse -> RIGHT);

	else if(traverse -> flag == 'r' && tTemp -> RIGHT && tTemp -> RIGHT -> flag == 'r')
		tTemp = findRightMost(tTemp -> RIGHT);
	*/

	if(newNode -> flag == 'r'){
		tTemp = findRightMost(newNode);
	}


	while((traverse -> flag == 'n' && traverse -> RIGHT) || (traverse -> flag == 'r' && findRightMost(traverse) -> RIGHT) ){

		if(traverse -> flag == 'r'){
			aTemp = findRightMost(traverse);
			whereToGo = 'r';
		}
		else
			whereToGo = 'n';


		if(newNode -> flag == 'n')
			nodeWhat = 'n';
		else
			nodeWhat = 'r';

		// init.

		if(whereToGo == 'n'){

			if(!isDivisibleArrayNode(values, traverse -> RIGHT, count, 2)){	// traverse has to be greater.

				if(nodeWhat == 'n' && traverse -> RIGHT -> flag == 'n'){
					newNode -> RIGHT = traverse -> RIGHT;
					traverse -> RIGHT = traverse -> RIGHT -> RIGHT;
					newNode -> RIGHT -> RIGHT = NULL;
					newNode = newNode -> RIGHT;
				}

				else if(nodeWhat == 'n' && traverse -> RIGHT -> flag == 'r'){
					NODE rightOfTraverse;
					REMNODE veryTemp = findRightMost(traverse -> RIGHT);
					rightOfTraverse = veryTemp -> RIGHT;

					newNode -> RIGHT = traverse -> RIGHT;
					traverse -> RIGHT = rightOfTraverse;
					veryTemp -> RIGHT = NULL;
					tTemp = veryTemp;
				}

				else if(nodeWhat == 'r' && traverse -> RIGHT -> flag == 'n'){
					tTemp -> RIGHT = traverse -> RIGHT;
					traverse -> RIGHT = traverse -> RIGHT -> RIGHT;
					tTemp -> RIGHT -> RIGHT= NULL;
					newNode = tTemp -> RIGHT;
				}

				else if(nodeWhat == 'r' && traverse -> RIGHT -> flag == 'r'){

					NODE rightOfTraverse;
					REMNODE veryTemp = findRightMost(traverse -> RIGHT);
					rightOfTraverse = veryTemp -> RIGHT;

					tTemp -> RIGHT = traverse -> RIGHT;
					traverse -> RIGHT = rightOfTraverse;
					veryTemp -> RIGHT = NULL;
					tTemp = veryTemp;
				}

			}

			else{
				mainNode -> qty += traverse -> RIGHT -> qty;
				traverse -> RIGHT -> val /= mainNode -> val;
				traverse = traverse -> RIGHT;
			}

		}


		else{

			if(!isDivisibleArrayNode(values, aTemp -> RIGHT , count, 2)){

				if(nodeWhat == 'n' && aTemp -> RIGHT -> flag == 'n'){
						newNode -> RIGHT = aTemp -> RIGHT;
						aTemp -> RIGHT = aTemp -> RIGHT -> RIGHT;
						newNode -> RIGHT -> RIGHT = NULL;
						newNode = newNode -> RIGHT;
				}

				else if(nodeWhat == 'n' && aTemp -> RIGHT -> flag == 'r'){
					NODE rightOfTraverse;
					REMNODE veryTemp = findRightMost(aTemp -> RIGHT);
					rightOfTraverse = veryTemp -> RIGHT;

					newNode -> RIGHT = aTemp -> RIGHT;
					aTemp -> RIGHT = rightOfTraverse;
					veryTemp -> RIGHT = NULL;
					tTemp = veryTemp;
				}

				else if(nodeWhat == 'r' && aTemp -> RIGHT -> flag == 'n'){
					tTemp -> RIGHT = aTemp -> RIGHT;
					aTemp -> RIGHT = aTemp -> RIGHT -> RIGHT;
					tTemp -> RIGHT -> RIGHT= NULL;
					newNode = tTemp -> RIGHT;
				}

				else if(nodeWhat == 'r' && aTemp -> RIGHT -> flag == 'r'){

					NODE rightOfaTemp;
					REMNODE veryTemp = findRightMost(aTemp -> RIGHT);
					rightOfaTemp = veryTemp -> RIGHT;

					tTemp -> RIGHT = aTemp -> RIGHT;
					aTemp -> RIGHT = rightOfaTemp;
					veryTemp -> RIGHT = NULL;
					tTemp = veryTemp;
				}
			}

			else{
				mainNode -> qty += aTemp -> RIGHT -> qty;
				aTemp -> RIGHT -> val /= mainNode -> val;
				traverse = aTemp -> RIGHT;
			}

		}

	}
}


// MEMORY CALCULATING FUNCTION

void findTotalMemoryUsage(NODE traverse){
	if(traverse == NULL)
		return;

	MemUsage += 29;
	REMNODE veryTemp;
	if(traverse -> flag == 'r'){
		MemUsage += 17;
		veryTemp = traverse -> REM;
		while(veryTemp -> flag == 'r'){
			veryTemp = veryTemp -> REM;
			MemUsage += 17;
		}
	}
}

void MemCalculate(NODE root){
	if(root == NULL)
		return ;

	MemCalculate(root -> CHILD);

	findTotalMemoryUsage(root);

	if(root -> flag == 'n')
		MemCalculate(root -> RIGHT);
	else
		MemCalculate(findRightMost(root) -> RIGHT);
}
