/* FILE: shm_prod.c */
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sysexits.h>
#include <unistd.h>
/* NOTE: These must be same on both prod and cons */
#define SHMNAME "/OS" //Name for our shared memory
#define SIZE (sizeof(int)*262144) //size of shared memory
//10 integers only.
/**
14 * @brief main function entry point
15 * @param[in] argc The count of arguments
16 * @param argv The arguments array
17 * @return normal exit or error.
18 */
int main(int argc, char const *argv[]){
//--Creating shared memory object--//
int shmObj = shm_open(SHMNAME, O_CREAT | O_RDWR , 0600);
//--Ask OS to reserve the sharem memory for 10 integers--//
ftruncate(shmObj,SIZE);
//--Ask the OS to MAP the memory to a pointer--//
int *ptrToShm = (int *)mmap(0, SIZE, PROT_WRITE | PROT_READ,MAP_SHARED, shmObj, 0);
/*NOTE:
28 *Now you can use ptrToShm as if it were a buffer dynamically allocated and
29 *read and write to it. Also we have explicitly cast the returned memory location
30 *from mmap as pointer to int. It can be anything char, double, float, etc.
31 *or any custom struct. But we will need to specifify the SIZE accordingly.
32 */
//--Writing 10 random numbers to our shared memory--//
for (int i = 0; i < 262144; ++i) {
	ptrToShm[i] = rand();
}
//--Print the shared memory--//
for (int i = 0; i < 262144; ++i) {
	printf("%d \n",ptrToShm[i]);
}
//--Safely release and close the shared memory--//
munmap(ptrToShm, SIZE);
close(shmObj);
shm_unlink(SHMNAME);
return EXIT_SUCCESS;
}
