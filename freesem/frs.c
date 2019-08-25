

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>






int main(int argc, char const *argv[])
{


	/*int semid = semget(7, 2, 0666 | IPC_CREAT | IPC_EXCL);
	printf("%d\n", semid);*/

	int count = 1000;
	for (int i = 0; i < count; ++i)
	{
		int semid = semget(i, 1, 0666 | IPC_CREAT |IPC_EXCL);
		if(semid == -1){
			printf("Key: %d\n", i);
			semid = semget(i, 1, 0666 | IPC_CREAT);
			semctl(semid, 0, IPC_RMID, 0);
		}
	}
	
		
	return 0;
}