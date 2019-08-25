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

	int ipckey = ftok(argv[0], 's');


	int semid = semget(ipckey, 1, 0666 | IPC_CREAT);
	if(semid < 0){
		perror("semget");
		exit(1);
	}
	semctl(semid, 0, SETVAL, 1);


	if((pid = fork()) == -1){
		perror("fork");
		exit(2);
	}
	printf("PID%d\n", pid );
	int which = 1;
	if(pid == 0){
		pid = getpid();
		which = 0;
	}
	if(which){
		semop(semid, &lock, 1);
		sleep(10);
		semop(semid, &unlock, 1);

	}else{
		semop(semid, &lock, 1);
		for (int i = 0; i < 100; ++i)
		{
			printf("df");
			printf("\a");
		}
	}

	/*int count = 10;
	printf("WHICH%d\n", which);
	printf("%d\n", semctl(semid, 0, GETVAL, 0));
	for(int i = 0; i < count; ++i){
		if (semop(semid, &lock, 1) == -1) {
			perror("semop");
			exit(3);
		}
		if(which){
			printf("O");
			fflush(stdout);
			sleep(5);
		}else{
			printf("X");
			fflush(stdout);
			sleep(2);
		}
		if (semop(semid, &unlock, 1) == -1) {
			perror("semop");
			exit(3);
		}
		sleep(1);
	}*/
	





	
	return 0;
}