#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/wait.h>
	 



int execvpe(const char *file, char *const argv[], char *const envp[]){

	//очистим окружение, дальше с помощью putenv зададим значения из envp
	clearenv();
	
	while(*envp){
		putenv(*envp);
		envp++;
	}
	//execvp можно, если добавить запускаемый файл в PATH
	//т к он ищет в PATH file
	execv(file, argv);
	return 0;
}


int main(int argc, char const *argv[])
{
	char* arr[] = {"PATH=cd", (char*)0};

	const char* arguments[] = {"arg"};

	int pid = 0;
	if(0 == (pid = fork())){
		execvpe("t.exe", arguments, arr);
		perror("Something goes wrong...");
		perror(0);
		exit(1);	
	}
	int s;
	wait(&s);
	return 0;
}