#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#include<string.h>
#include<sys/shm.h>
#include<memory.h>



int main(int argc, char const *argv[])
{


	/*int semid = semget(7, 2, 0666 | IPC_CREAT | IPC_EXCL);
	printf("%d\n", semid);*/

	int count = 2000;
	for (int i = 1000; i < count; ++i)
	{
		int shmid = shmget(i, 1 , 0600|IPC_CREAT | IPC_EXCL);
		if(shmid == -1){
			printf("Key: %d\n", i);
			shmid = shmget(i, 1 , 0600|IPC_CREAT);
			shmctl(shmid, IPC_RMID, NULL);
		}
	}
	
		
	return 0;
}