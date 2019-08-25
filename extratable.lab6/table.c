#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/times.h>
#include <time.h>


void showInfo(char* buf , int fdin , int all){
	int n = 0 , end = 0;
	while((n = read(fdin, buf, BUFSIZ)) > 0){
		for (int i = 0; i < n; ++i){
			if(!all && '\n' == buf[i]){
				end = 1;
				break;
			}
			write(1, buf + i , 1);
		}
		printf("\n");
		if(1 == end) break;
	}
}

int main(int argc, char const *argv[])
{
	if(argc != 2){
       fprintf(stderr , "Invalid arguments!\n");     
	   return 0;
	}

	int strseek[100] = {0};
	char buf[BUFSIZ];
	int fdin  = 0;


	if(-1 == (fdin = open(argv[1] ,  O_RDONLY))){
		perror(argv[1]);
		exit(2);//?
	}

	int s_pos = 0 , lines_c = 0 , n = 0;
	while ((n = read(fdin, buf, BUFSIZ)) > 0){
		for (int i = 0; i < n; ++i)
		{
			if('\n' == buf[i]){
				++lines_c;
				strseek[lines_c] = s_pos + 1;
			}
			++s_pos;
		}
	}
	if(0 == lines_c){
		printf("File is empty!\n");
		return 0;
	}

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
			lseek(fdin , 0 , SEEK_SET);
			printf("--------------\n");
			showInfo(buf , fdin , 1);
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

		lseek(fdin , strseek[l_num - 1] , SEEK_SET);
		showInfo(buf , fdin , 0);
	}

	close(fdin);
	return 0;
}