#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#include<string.h>
#include<sys/shm.h>
#include<memory.h>

//shared memory - shm
#define BUF_SIZE 100
#define Q_SIZE 10


//семафоры продолжают жить сло своими значениями, как их убить?При аварийном завершении.

typedef struct MSG{
	char buf[BUF_SIZE];
	int size;
}MSG;

char* shm_ptr;
int semid, shmid;


#define FULL  0
#define EMPTY  1
#define SHARED 2
#define KEY 41

void lock(int semi);
void unlock(int semi);


void init_segment(){
	shmid = shmget(KEY, Q_SIZE * sizeof(MSG) + 2 * sizeof(int) , 0600|IPC_CREAT);

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
	semid = semget(KEY, 3, 0666 | IPC_CREAT);
	if(semid == -1){
		perror("semget failed");
		exit(3);
	}
	unlock(SHARED);//просто увеличиваем на 1
}

void run(){
	//char buf[BUF_SIZE];
	int* head =  (int*)(shm_ptr + Q_SIZE * sizeof(MSG));
	int* tail = (int*)(shm_ptr + Q_SIZE * sizeof(MSG) + sizeof(int));
	MSG* q = (MSG*)shm_ptr;
	*head = 0;
	*tail = 0;
	MSG m;
	while(1){
		int size = read(0, m.buf, BUF_SIZE - 1);


		if(size == -1){
			perror("read");
			exit(4);
		}

		m.buf[size] = '\0';//?
		m.size = size;
		lock(SHARED);
		q[*tail] = m;
		++(*tail);

		if(*tail - 1 == *head) unlock(EMPTY);

		if(*tail == Q_SIZE){
			*tail = 0;
		}
		if(*tail == *head){
			printf("Do not write &_&.\n");
			unlock(SHARED);
			lock(FULL);
		}else{
			unlock(SHARED);
		}
		if(m.buf[0] == '0'){
			return;
		}
	}
}

int main(int argc, char const *argv[])
{

	
	init_segment();
	init_sem();

	run();


	//semctl(semid, 0, IPC_RMID, 0);
	shmctl(shmid, IPC_RMID, NULL);

	return 0;
}

void changesem(int semi, int c){
	struct sembuf lock = { semi, -1, SEM_UNDO };
	struct sembuf unlock = { semi, 1, SEM_UNDO };
	struct sembuf* st;
	if(c) st = &lock;
	else st = &unlock;
	if (semop(semid, st, 1) == -1) {
		perror("semop prod");
		exit(3);
	}
}


void lock(int semi){
	changesem(semi , 1);
}


void unlock(int semi){
	changesem(semi, 0);
}