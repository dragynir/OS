#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>








int main(int argc, char const *argv[])
{

	static struct sembuf lock = { 0, -1, SEM_UNDO };
	static struct sembuf unlock = { 0, 1, SEM_UNDO };


	int pid = 0;

	int ipckey = ftok("cat", 's');


	int semid = semget(ipckey, 1, 0666 | IPC_CREAT);
	semctl(semid, 0, SETVAL, 1);

	int count = 10;

	printf("%d\n", semctl(semid, 0, GETVAL, 0));
	for(int i = 0; i < count; ++i){
		if (semop(semid, &lock, 1) == -1) {
			perror("semop");
			exit(3);
		}
		printf("X");
		fflush(stdout);
		sleep(2);
		if (semop(semid, &unlock, 1) == -1) {
			perror("semop");
			exit(3);
		}
	}
	
	return 0;
}