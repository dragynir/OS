
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
//https://habr.com/ru/post/195152/
//http://www.taketop.ru/articles/informatika/operac-sis/pipe


//linux однонаправленный канал
int main(int argc, char const *argv[])
{
	
	if(argc!=2){
		printf("%s\n", "Requared msg");
		exit(1);
	}

	int MSG_SIZE = strlen(argv[1]);
	const char* msg = argv[1];
	

	int fd[2];
	pid_t pid;

	if(pipe(fd) == -1){
		perror(0);
		exit(2);
	}
	//1 write
	//0 read
	if((pid = fork()) > 0){
		write(fd[1], msg, MSG_SIZE);
	}else if(pid == 0){
		char* out = calloc(MSG_SIZE, 1);
		read(fd[0], out, MSG_SIZE);
		for(int i = 0; i < MSG_SIZE; ++i){
			out[i] = toupper(out[i]);
		}
		printf("%s\n", out);
		free(out);
	}else{
		perror(0);
		exit(3);
	}		
	return 0;
}
