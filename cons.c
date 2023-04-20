#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <iostream>
using namespace std;
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1;i++)
        L[i] = arr[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
 
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if(L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        }
        else{
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while(i < n1){
        arr[k] = L[i];
        i++;
        k++;
    }
    while(j < n2){
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            merge_sort(arr, l, m);
            _exit(EXIT_SUCCESS);
        }
        else {
            merge_sort(arr, m + 1, r);
            int status;
            waitpid(pid, &status, 0);
        }
        merge(arr, l, m, r);
    }
}

int main(int argc, char* argv[]){
    int *shared_memory;
    int shmid;
    shmid=shmget((key_t)1122, sizeof(int) * 8, 0666|IPC_EXCL);
    //cout <<"The key of the shared memory is: "<<shmid<<endl;
    shared_memory = (int *)shmat(shmid, NULL, 0);
    //cout <<"The process is attaced at: "<< shared_memory<<endl;
    merge_sort(shared_memory, 0, 3);

    cout<<"sorting left side: "<<endl;
    for(int i = 0; i < 8; i++)
        cout<<shared_memory[i]<<", ";

    return 0;
}
