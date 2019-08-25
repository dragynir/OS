#include <stdio.h>
#include <unistd.h>
#include <termios.h>
//неканоничный http://www.igce.comcor.ru/non_canon.html
//c_lflag
//http://ccfit.nsu.ru/~deviv/courses/unix/unix/ng6c2d8.html
int main() {
	/*if (!isatty(0)) {//открыт(ассоциирован) ли stdin с терминалом
		perror("No \n");
		return -1;
	}*/

	char* q = "Would you like to help a cat? y/n";
	struct termios old_tty;
	struct termios tty;
	//зачем
	const int STDIN = fileno(stdin);

	printf("%s\n", q);
	tcgetattr(STDIN, &old_tty);
	
	tty = old_tty;


	//отключаем канонический ввод(ICANON)
	//убираем сравнение символов сигналов с INTR QUIT ...
	tty.c_lflag &= ~(ISIG | ICANON);//change input modes(c_flag is tcflag_t)
	tty.c_cc[VMIN] = 1; // one char

	//установить немедленно
	//зачем немедленно
	tcsetattr(STDIN, TCSANOW, &tty); 

	char answer[4];
	while (1) {
		read(STDIN, &answer, 4);
		

		switch (answer[0]) {
			case 'y':
				printf("\n");
				//TCSANOW немедленно ставим атрибуты
				tcsetattr(STDIN, TCSANOW, &old_tty);
				return 0;
			case 'n':
				printf("\n%s\n", q);
				break;
			default :
				printf("\nHelp or not?!\n");
		}
	}
	
	return 0;
}
