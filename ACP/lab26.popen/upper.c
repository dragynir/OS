#include<stdio.h>
#include<ctype.h>

#define MSG_SIZE 10

int main(int argc, char const *argv[])
{
	char msg[MSG_SIZE];
	fgets(msg, MSG_SIZE, stdin);
	for(int i = 0; i < MSG_SIZE; ++i){
		msg[i] = toupper(msg[i]);
	}
	printf("%s\n", msg);
	return 0;
}