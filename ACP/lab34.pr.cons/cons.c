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

char* shm_ptr;
int semid, shmid;

//produce consumer
#define PROD_SEM  0
#define CONS_SEM  1
#define KEY 99
void lock(int semi);
void unlock(int semi);



void get_segment(){
	shmid = shmget(KEY, BUF_SIZE, 0600|IPC_CREAT);

	if(shmid == -1){
		perror("shmget failed");
		exit(1);
	}
	shm_ptr = shmat(shmid, 0, 0);
	if(shm_ptr == (char*)-1){
		perror("shmat");
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

void run(){
	char msg[BUF_SIZE];
	while(1){
		lock(CONS_SEM);
		for (int i = 0; i < BUF_SIZE; ++i)
		{
			msg[i] = shm_ptr[i];
		}
		printf("%s\n", msg);
		unlock(PROD_SEM);
	}

}

int main(int argc, char const *argv[])
{

	
	get_segment();
	init_sem();

	run();

	shmdt(shm_ptr);
	semctl(semid, 0, IPC_RMID, 0);


	return 0;
}



void changesem(int semi, int c){
	struct sembuf lock = { semi, -1, SEM_UNDO };
	struct sembuf unlock = { semi, 1, SEM_UNDO };
	struct sembuf* st;
	if(c) st = &lock;
	else st = &unlock;
	if (semop(semid, st, 1) == -1) {
		perror("semop");
		exit(3);
	}
}


void lock(int semi){
	changesem(semi , 1);
}


void unlock(int semi){
	changesem(semi, 0);
}