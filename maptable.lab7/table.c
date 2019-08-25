#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/times.h>
#include <time.h>
#include<sys/mman.h>
#include<sys/stat.h>

void showInfo(char* buf , int start , int end){

	for (int i = start; i < end; ++i)
	{
		write(1, buf + i , 1);
	}
}


/*

a\n -  две строки

*/

int main(int argc, char const *argv[])
{
	if(argc != 2){
       fprintf(stderr , "Invalid arguments!\n");     
	   return 0;
	}

	int strseek[101] = {0};
	char buf[BUFSIZ];
	int fdin  = 0;


	if(-1 == (fdin = open(argv[1] ,  O_RDONLY))){
		perror(argv[1]);
		exit(2);//?
	}
	struct stat statbuf;
	fstat(fdin, &statbuf);
	int size = statbuf.st_size;
	char* frep = (char*)mmap(0 , size , PROT_READ ,MAP_SHARED, fdin , 0);
	if((char*)-1 == frep){
		perror(0);
	}
 
	int s_pos = 0 , lines_c = 0 , n = 0;
	for (int i = 0; i < size; ++i){
		if('\n' == *(frep + i)){
			++lines_c;
			strseek[lines_c] = s_pos + 1;
		}
		++s_pos;	
	}

	if(0 == lines_c){
		printf("File is empty!\n");
		return 0;
	}
	strseek[lines_c + 1] = size;

	int l_num = 0;

	//?
	struct timeval timeout; 
	int len = 0, retval = 0; 
	fd_set readfd; 
	timeout.tv_sec = 5; 
	timeout.tv_usec = 0; 
	FD_ZERO(&readfd); 
	FD_SET(0, &readfd); 

	while(1){

		retval = select(1, &readfd, NULL, NULL, &timeout); 
		
		if(0 == retval){
			printf("--------------\n");
			showInfo(frep , 0 , size);
			return 0;
		}else{
			timeout.tv_sec = 5; 
			timeout.tv_usec = 0; 
		}

		if(0 == scanf("%d" , &l_num)){
			char c = 0;
			scanf("%c" , &c);
			continue;
		}

		if(l_num > lines_c || l_num < 0){
			printf("Lines in range: %d , %d\n", 1 , lines_c);
			continue;
		}

		if(0 == l_num){
			printf("Deny...\n");
			return 0;
		}
		showInfo(frep , strseek[l_num - 1] , strseek[l_num]);
	}

	close(fdin);
	return 0;
}