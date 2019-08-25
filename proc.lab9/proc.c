
#include<fcntl.h>
#include <sys/types.h>
#include<string.h>

#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include<sys/wait.h>


//fork
//http://citforum.ru/operating_systems/bach/glava_66.shtml
/*Если контекст порожденного процесса готов,
 родительский процесс завершает свою роль в выполнении алгоритма fork,
  переводя порожденный процесс в состояние
   "готовности к запуску, находясь в памяти" 
   и возвращая пользователю его идентификатор.*/

//vfork
//http://wm-help.net/lib/b/book/276807463/149

//exec зомби 
//https://www.ibm.com/developerworks/ru/library/au-unixprocess/index.html

int main(int argc, char const *argv[])
{

	int pid = 0;
	int s = 0;
	/*if((pid = fork()) > 0){
		printf("Hello I am Here\n");
	}else if(0 == pid){
		system("cat text.txt");
	}else{
		perror(0);
	}*/
	int i = open("text.txt" , O_RDONLY);
	char buf[4];
	if((pid = fork()) == 0){
		read(i , buf ,4);
		printf("%sChild\n" , buf );
		exit(0);
	}else if(pid >0){
		read(i , buf ,4);
		printf("%sParent\n" , buf );
	}else{
		perror(0);
	}

	/*int r = wait(&s);
	printf("\n!!!!!!!!!!!!!!!!!!\n");
	printf("Ret v: %d\n", r);*/
	return 0;
}