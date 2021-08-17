
#include "modules.h"
#include "timSort.h"

void insertIntoTree(NODE root, int* values, int count){
	
	// NOT THERE PREVIOUSLY  
	static int e=0;
	e++;
	// NOT THERE PREVIOUSLY

	if(root -> CHILD == NULL){
		NODE newNode = createNode(values, count, 1, NULL, NULL);
		root -> CHILD = newNode;
	} // first node

	else{
		NODE traverse = root -> CHILD, prev = root;
		int complete = 0, tempValue;

		while(!complete){

			tempValue = isDivisibleArrayNode(values, traverse, count, 0);
			
			// case 1 - is Divisible
			if(tempValue){

				if(tempValue == -1){
					traverse -> qty++;
					complete = 1;
					return;
				}

				else if(tempValue == 1){
					if(traverse -> CHILD != NULL){
						removeFactorsArray(&values, &traverse, &count, 1);
						prev -> CHILD = traverse;
						prev = traverse;
						traverse -> qty++;
						traverse = traverse -> CHILD;
						continue;
					}
				}


				int divisible = tempValue;

				if(divisible){
					NODE newNode;
				 	if(divisible == 1){

						removeFactorsArray(&values, &traverse, &count,1);	// remove from values.
						prev -> CHILD = traverse;

						newNode = createNode(values, count, 1, NULL, NULL);
						traverse -> CHILD = newNode;
						traverse -> qty++;
						complete = 1;
					}

					else{
						removeFactorsArray(&values,&traverse,&count,2);	 // remove from traverse.
						newNode = createNode(values, count, traverse -> qty + 1, traverse, NULL);
						prev -> CHILD = newNode;
						moveNonFactorsRight(newNode,traverse,values,count);
						complete = 1;
					}
				}

				else{
					continue;
				}
			}


			//case 2  - not divisible
			else{
				int flag = -1;
				NODE travTemp;
				REMNODE veryTemp;
				while(flag == -1){
					
					if(traverse -> flag == 'r'){
						veryTemp = findRightMost(traverse);     // 3rd function
						travTemp = veryTemp -> RIGHT;
					}
					else{
						veryTemp = NULL;
						travTemp = traverse -> RIGHT;
					}
					
					if(travTemp == NULL)
						break;

					else{ // if encountered is r.
						int x = isDivisibleArrayNode(values, travTemp, count, 0); // returns 0 if not div, -1 if equal
						if(x == -1){
							travTemp -> qty++;
							return;
						}
						else if(x){
							flag = (x == 1)? 2 : 1;
						}
						else
							traverse = travTemp; //this is alright i think
					}

				}

				if(flag){
					NODE newNode;
					switch(flag){
						case -1:
							newNode = createNode(values, count, 1, NULL, NULL);
							if(!veryTemp)
								traverse -> RIGHT = newNode;
							else{
								veryTemp -> RIGHT = newNode;
							}
							complete = 1;
							break;

						case 1:
							newNode = createNode(values, count, 1, NULL, NULL);
							newNode -> qty += travTemp -> qty;
							if(!veryTemp)
								traverse -> RIGHT = newNode;
							else
								veryTemp -> RIGHT = newNode;

							removeCommonFactorsNodes(&travTemp, newNode);
							newNode -> CHILD = travTemp;

							moveNonFactorsRight(newNode, travTemp, values, count);
							complete = 1;
							break;

						case 2:
							travTemp -> qty++;
							if(travTemp -> CHILD == NULL){
								newNode = createNode(values, count, 1, NULL, NULL);// create and add here.
								removeCommonFactorsNodes(&newNode, travTemp);
								travTemp -> CHILD = newNode;
								complete = 1;
							}
							else{
								prev = travTemp;
								removeFactorsArray(&values, &travTemp, &count, 1);
								traverse = travTemp-> CHILD;
								continue;
							}
							break;
					}
				}
			}

		}// close of while(!complete) loop

	} // else close (main else)

}


NODE PWCTreeConstruction(int rows, int &insertedRows){

	NODE root;
	root = (NODE)malloc(sizeof(struct node));
	root -> val = 0;
	root -> qty = 0;
	root -> flag = 'R';
	root -> CHILD = root -> RIGHT = NULL;

	// reading all the primes.
	allPrimes = (int*)malloc(limitPrime * sizeof(int));
	FILE *fp = fopen(primeFile, "r");
	char *line = (char*)malloc(sizeof(char) * 20);

	int x = 0;
	while(x < limitPrime){
		fgets(line, 20000, fp);
		const char* token;
		token = strtok(line, ",");
		allPrimes[x++] = atoi(token);
	}
	free(line);

	//reading the data
	int *data = (int*)malloc(sizeof(int) * MAX_LIMIT);
	int cols;

	FILE* stream = fopen(fileName, "r");
	int i = 0, j;
    	line = (char*)malloc(sizeof(char) * 10000);

	//fgets(line, 10000, stream);  // removes first line from the reading

	unsigned int mainI = 0;

	unsigned int greaterThanPrev;
	for(int i = 0; i < rows; i++){

		if(fgets(line, 10000, stream) == NULL)
			break;
		cols = 0;
		char* tmp = strdup(line);

		const char* tok = strtok(tmp, ",");
		greaterThanPrev = 1;
		//tok = strtok(NULL, ",\n")
		for (; tok && *tok; tok = strtok(NULL, ",\n")){
			data[cols++] = allPrimes[atoi(tok)];
			if(greaterThanPrev){
				if(greaterThanPrev < data[cols - 1])
					greaterThanPrev = data[cols - 1];
				else
					greaterThanPrev = 0;
			}
		}

		unsigned long int prod = 1;
		for(int j = 0; j < cols; j++){
			if(INT_MAX_FINAL / prod > data[j]){
				prod *= data[j];
			}
			else{
				cols = 21;
				break;
			}
		}


		if(cols < 20){
       		free(tmp);
			if(!greaterThanPrev)
				sort(data,cols);

			insertIntoTree(root, data, cols);
			insertedRows++;
		}
		else
			i--;

		mainI++;
	}
	free(line);
	fclose(stream);

	// printf("Nodes Traversed : %d\n", tempMem);
	// printf("Actual I : %d\n\n", mainI);

	return root;
}
