#include "mining.h"

int main(){

    int rows, rareThreshold, freqThreshold;
    cout << "Enter the number of rows: ";
    cin >> rows;
    cout << "Enter the rare threshold: ";
    cin >> rareThreshold;
    cout << "Enter the frequent threshold: ";
    cin >> freqThreshold;

    int rareRows = (rareThreshold * rows) / 100;
    int freqRows = (freqThreshold * rows) / 100;

    clock_t start, end;
    double cpu_time_used;
	start = clock();

    // STEP 1: CREATE THE TREE
    NODE root = PWCTreeConstruction(rows);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\nINSERTION = %lf seconds\n", cpu_time_used);

    // STEP 2: MINE THE TREE
    map<vector<int>, int> freqTable;
    mineTree(root -> CHILD, vector<vector<int>>(), freqTable);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("INSERTION + MINING = %lf seconds\n", cpu_time_used);

    // STEP 3: PICK THE FREQUENT AND RARE ITEMSETS

    ofstream frequent, rare;
    frequent.open("./output/frequent.txt");
    rare.open("./output/rare.txt");

    int primePos;
    for(auto i : freqTable){
        if(i.second > freqRows){
            for(int j = 0; j < i.first.size(); j++){
                isPrime(i.first[j], &primePos);
                frequent << primePos << ',';
            }
            frequent << "COUNT = " << i.second << endl;
        }

        else if(i.second > rareRows && i.second < freqRows){
            for(int j = 0; j < i.first.size(); j++){
                isPrime(i.first[j], &primePos);
                rare << primePos << ',';
            }
            rare << "COUNT = " << i.second << endl;
        }
    }
    cout << endl << "Frequent and Rare Itemsets are generated in the output folder." << endl << endl;
}
