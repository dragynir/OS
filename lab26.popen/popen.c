

#include<stdio.h>


#define MSG_SIZE 10

//http://qaru.site/questions/14062975/how-does-popen-work-and-how-to-implement-it-into-c-code-on-linux
//возможная реализация на freopen()

int main(int argc, char const *argv[])
{
	char msg[MSG_SIZE] = {"some msg1"};

	FILE* fpout = popen("./upper", "w");

	fputs(msg,fpout);
	pclose(fpout);
	return 0;
}
