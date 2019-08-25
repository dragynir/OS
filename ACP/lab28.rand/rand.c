

#include<stdio.h>
#include<stdlib.h>

//ONLY IN SVR4
#include<libgen.h>

//gcc rand.c -o s -std=c99 -lgen





int main(int argc, char const *argv[])
{
	srand(5);
	FILE* fptrs[2];

	p2open("sort -g", fptrs);

	for(int r = 0; r < 100; ++r){
		fprintf(fptrs[0], "%d ", rand());
	}
	fclose(fptrs[0]);

	char buf[BUFSIZ];
	//int n;
	while(fgets(buf, BUFSIZ, fptrs[1]) != NULL){
		printf("%s\n", buf);
	}
	
	return 0;
}