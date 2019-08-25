#include<stdio.h>


extern char **environ;

int main(int argc, char const *argv[])
{
	char** e = environ;
	
	while(*e){
		printf("%s\n", *e);
		e++;
	}
	return 0;
}