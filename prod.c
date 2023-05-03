#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;
bool check_sorted(int list[], int s,int n){
    for(int i = s; i < n - 1 ; i++){
        //if the current element is larger than the
        //adjacent one, it means that the list is 
        //sorted
        if(list[i] > list[i + 1])
            return false;
    }
    return true;
}

void merge(int arr[], int l, int m, int r){
    //merge sort array
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
    are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
    are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
int main(int argc, char* argv[]){
    srand(time(NULL));
    int *shared_memory;
    int shmid;
    //creating the shared memory
    shmid = shmget((key_t)1122, sizeof(int) * 8, 0666|IPC_CREAT);
    //shmid = shmget((key_t)1122, sizeof(int) * 262144, 0666|IPC_CREAT);
    //we will associate it with a variable
    shared_memory = (int *)shmat(shmid, NULL, 0);
    
    int pid = fork();
    if (pid == 0) { // Child process
        //we check if the both halves are sorted or not
        //if(check_sorted(shared_memory, 0,131072) && check_sorted(shared_memory,131072, 262143)){
        if(check_sorted(shared_memory, 0, 4) && check_sorted(shared_memory,4, 8)){
            cout<<"List is fit for merging"<<endl;
            //then we will sort it one final time
            merge(shared_memory, 0, 3, 7);
            cout<<"sorted array:"<<endl;
            //printing the array
            for(int i = 0; i < 8; i++){
               cout<<shared_memory[i]<<", ";
            }
            cout<<endl;
        }
        
        //tells user to run the consumers if the array is not sorted.
         //if(!check_sorted(shared_memory, 0,131072) && !check_sorted(shared_memory,131072, 262143)){
        if(!check_sorted(shared_memory,4, 8) || !check_sorted(shared_memory, 0, 4))
            cout<< "Run consumer to sort the array"<<endl;     
         
    }
    else if (pid > 0 ) { // Parent process
        cout<< "Waiting for child "<<endl ;
        //wait for the child to check the array
        //otherwise generate a new array
        wait(NULL);
        for(int i = 0; i < 8; i++)
            shared_memory[i] = rand() % 100; 
        
        for(int i = 0; i < 8; i++){
           cout<<shared_memory[i]<<", ";
        }
        //deallocating memory
        shmdt((void *) shared_memory);
    }
    else { // Fork failed
        cerr << "Fork failed" << endl;
        return 1;
    }
    return 0;
}
