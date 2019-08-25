#include<stdlib.h>
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>


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

	int l_num = 0;
	while(1){
		if(0 == scanf("%d" , &l_num)){
			lseek(0 , 0 , SEEK_END);
			char c = 0;
			scanf("%c" , &c);
			continue;
		}

		if(l_num > lines_c || l_num < 0){
			fprintf( stderr , "Lines in range: %d , %d\n", 1 , lines_c);
			continue;
		}
		if(0 == l_num){
			printf("Deny...\n");
			return 0;
		}

		lseek(fdin , strseek[l_num - 1] , SEEK_SET);
		char s = 0;
		int end = 0;
		while((n = read(fdin, buf, BUFSIZ)) > 0){
			for (int i = 0; i < n; ++i){
				if('\n' == buf[i]){
					end = 1;
					break;
				}
				write(1, buf + i , 1);
			}
			printf("\n");
			if(1 == end) break;
		}
	}

	close(fdin);
	return 0;
}