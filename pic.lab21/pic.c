#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>


struct termios old_tty;
static int count = 0;

void sigcatch(int signal) {
	printf("total: %d\n", count);
	tcsetattr(fileno(stdin), TCSANOW, &old_tty);
	exit(0);
}

int main() {
	struct termios tty;

	tcgetattr(fileno(stdin), &old_tty);

	tty = old_tty;
	tty.c_lflag &= ~(ECHO | ICANON);
	
	tcsetattr(fileno(stdin), TCSANOW, &tty);
	char c[4];
	while (1) {
		signal(SIGINT, sigcatch);
		read(1,c,4);
		if(c[0] == 27 && c[1] == 91 &&c[2] == 51 && c[3] == 126){
			write(1,"\a" ,1);
			++count; 	
			//sleep(1);

		}
	}
	tcsetattr(fileno(stdin), TCSANOW, &old_tty);

	return 0;
}
