#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#include<sys/shm.h>
#include<memory.h>

//shared memory - shm
#define BUF_SIZE 10
const int CONS_COUNT = 3;


char* shm_ptr;
int semid, shmid;

//produce consumer
#define PROD_SEM  0
#define CONS_SEM  1
#define KEY 20
void lock(int semi);
void unlock(int semi);
void longlock(int semi);
void longunlock(int semi);


void init_segment(){
	shmid = shmget(KEY, BUF_SIZE, 0600|IPC_CREAT);

	if(shmid == -1){
		perror("shmget failed");
		exit(1);
	}
	shm_ptr = shmat(shmid, 0, 0);

	if(shm_ptr == (char *)-1){
		perror("shmat failed");
		exit(2);
	}
}


void init_sem(){
	semid = semget(KEY, 2, 0666 | IPC_CREAT);
	if(semid == -1){
		perror("semget failed");
		exit(3);
	}
}

void run_prod(){
	printf("product start\n");
	while(1){
		sleep(2);
		for(int i = 0; i < BUF_SIZE - 1; ++i){
			shm_ptr[i] =  48 + rand() % 56;
		}
		shm_ptr[BUF_SIZE] = '\0';
		printf("MSG ready %s\n", shm_ptr);
		longunlock(CONS_SEM);
		longlock(PROD_SEM);
	}	

}

void run_cons(){
	char msg[BUF_SIZE];
	while(1){
		lock(CONS_SEM);
		for (int i = 0; i < BUF_SIZE; ++i)
		{
			msg[i] = shm_ptr[i];
		}
		printf("MSG: %s\n", msg);
		unlock(PROD_SEM);
	}
	shmctl(shmid, IPC_RMID, NULL);
}




int main(int argc, char const *argv[])
{


	int sic = semget(KEY, 1, IPC_CREAT | 0666);
	semctl(sic, 0, IPC_RMID, 0);
	printf("RM %d\n",sic);
	//return 0;
	
	init_segment();
	init_sem();

	

	int pid = 1;
	for (int i = 0; i < CONS_COUNT; ++i)
	{
		if((pid = fork()) == 0){
			run_cons();
			break;
		}		
	}
	if(pid != 0) run_prod();

	if(pid != 0){
		semctl(semid, 0, IPC_RMID, 0);
		shmctl(shmid, IPC_RMID, NULL);
	}


	return 0;
		
}



void changesem(int semi, int c){
	struct sembuf lock = { semi, -1, SEM_UNDO };
	struct sembuf unlock = { semi, 1, SEM_UNDO };
	struct sembuf decr = {semi, -CONS_COUNT, SEM_UNDO };
	struct sembuf incr = {semi, CONS_COUNT, SEM_UNDO };

	struct sembuf* st;
	if(c == 1) st = &lock;
	else if(c == 0)st = &unlock;
	else if(c == 2)st = &decr;
	else st = &incr;

	if (semop(semid, st, 1) == -1) {
		perror("semop");
		exit(3);
	}
}


void lock(int semi){
	changesem(semi , 1);
}

void longlock(int semi){
	changesem(semi, 2);
}

void longunlock(int semi){
	changesem(semi, 3);
}


void unlock(int semi){
	changesem(semi, 0);
}