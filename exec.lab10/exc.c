

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>


// отличии от запускаемой программы, программа , поражденная с помощью exec
//получает в качестве argv[0] параметры , начинающиеся с params в execv

//*exec
//https://studfiles.net/preview/2774686/page:18/
//http://www.codenet.ru/progr/cpp/spr/084.php
//wait
//https://linux.die.net/man/3/wait
int main(int argc, char *argv[])
{

	if(argc < 2){
		printf("Wrong params.\n");
		return 0;
	}

	char* _arguments[] = {"/bin/cat" , "text.txt" , (char*)0};

	int pid = 0 , s = 0;
	if(0 == (pid = fork())){
		execv( argv[1] , _arguments);
		perror("Something goes wrong...");
		exit(1);	
	}else if(pid < 0){
		perror(0);
	}
	wait(&s);
	//s - код по которуму можно узнать о том
	//как завершился дочерний процесс
	//возвращает wait pid завершенного процесса
	printf("Ret val: %d\n", WIFEXITED(s));//non-zero value if child process terminated normally
	return 0;
}