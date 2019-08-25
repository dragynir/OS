#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
int main(int argc, char const *argv[])
{
	if(argc != 2){
		printf("%s\n", "File path expected.");
		exit(1);
	}
	int f = open(argv[1], O_RDONLY);
	if(f == -1){
		perror(0);
		exit(2);
	}
	//512
	char buf[BUFSIZ + 1];
	//^ начало строки,  $ конец строки
	FILE* fpout = popen("grep '^\\s*$' | wc -l", "w");
	int n;
	while((n = read(f, buf, BUFSIZ)) > 0){
		buf[n] = '\0';
		fputs(buf,fpout);
	}	
	pclose(fpout);
	return 0;
}