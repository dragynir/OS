#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
//ЭТО ОНО

const int A = 0, B = 1, C = 2, MOD1 = 3, WIDG = 4;
const int A_TIME = 2, B_TIME = 3 , C_TIME = 4, WIDG_TIME = 2;

void changesem(int semid, int semi, int c){
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


void dec(int semid, int semi){
	changesem(semid, semi , 1);
}


void inc(int semid, int semi){
	changesem(semid, semi, 0);
}



void lineA(int semid){
	while(1){
		sleep(A_TIME);
		inc(semid, A);
		dec(semid,MOD1);
	}
}


void lineB(int semid){
	while(1){
		sleep(B_TIME);
		inc(semid, B);
		dec(semid,MOD1);
	}
}



void lineC(int semid){
	while(1){
		sleep(C_TIME);
		inc(semid, C);
		dec(semid,WIDG);
	}
}


//удаление семафоров?
//http://rus-linux.net/MyLDP/BOOKS/lpg-04/node6.htm#SECTION00743000000000000000
//https://www.opennet.ru/docs/RUS/linux_base/node229.html


//есть поведение:
//если набор семафоров с заданным ключом существует, и вы патаетесь получить 
// semid с флагом IPC_CREAT, прося больше семафоров чем есть, errno InvalidArgument
int main(int argc, char const *argv[])
{

	int pid = 0;

	int ipckey = 1;//ftok(argv[0], 's');
	int pids[3];	
	//printf("%d\n", _SC_SEM_NSEMS_MAX );

	/*int sic = semget(ipckey, 1, IPC_CREAT | 0666);
	semctl(sic, 0, IPC_RMID, 0);
	return 0;*/

	int semid = semget(ipckey, 5, IPC_CREAT | 0666);
	//semop(int semid, struct sembuf *sops, unsigned nsops);
	

	if(semid < 0){
		perror("semget here");
		exit(1);
	}

	//unlock(semid,A);

	//lock(semid, A);


	//получить значенике A семафора
	//printf("%d", semctl(semid, A, GETVAL, 0));

	

	void (*arr[3])(int) = {lineA, lineB, lineC};

	for (int i = 0; i < 3; ++i)
	{
		if((pid = fork()) == -1){
			perror("fork");
			exit(2);
		}else if(pid == 0){
			arr[i](semid);
			break;
		}else{
			pids[i] = pid;
		}
	}



	while(1){
		dec(semid,A);
		printf("A: \\\n");
		dec(semid,B);
		printf("B: /\n");


		inc(semid,MOD1);
		inc(semid,MOD1);
		printf("MOD1: \\_/\n");


		dec(semid,C);
		printf("C: *\n");

		sleep(WIDG_TIME);

		inc(semid,WIDG);
		printf("WIDG: *\\_/*\n");
		printf("--------->\n\n");
	}


	semctl(semid, 0, IPC_RMID, 0);
	
	return 0;
}