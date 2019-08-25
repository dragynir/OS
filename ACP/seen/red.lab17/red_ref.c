#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define MAX_LINE 41

int  eraseWord(int STDIN, char* line, int count){
	if(count == 0) return 0;
	char back = '\r';
	write(1,&back,1);
	int s = 0;
	for(s = count - 1; s > 0; --s){
		if(line[s] != ' ' && line[s - 1] == ' '){
			break;
		}
	}
	for (int i = 0; i < count ; ++i)
	{	
		if(i >= s){
			line[i] = '\0';
		}
		write(1,&line[i],1);
	}
	write(1,&back,1);
	for (int i = 0; i < s ; ++i)
	{	
		write(1,&line[i],1);
	}
	return s;
}

int eraseLine(int STDIN, char* line, int count){
	if(count == 0) return 0;
	char back = '\r';
	write(1,&back,1);

	for (int i = 0; i < count; ++i)
	{
		line[i] = '\0';
		write(1,&line[i],1);
	}
	write(1,&back,1);
}

int  eraseSimbol(int STDIN, char* line, int count){
	if(count == 0) return 0;
	char back = '\r';
	write(1,&back,1);

	line[count - 1] = '\0';
	for (int i = 0; i < count; ++i)
	{
		write(1,&line[i],1);
	}
	write(1,&back,1);
	for (int i = 0; i < count - 1; ++i)
	{
		write(1,&line[i],1);
	}
	return count - 1;
}

int main(int argc, char const *argv[])
{

	const int STDIN = fileno(stdin);

	struct termios old_tty;
	struct termios tty;

	tcgetattr(STDIN, &old_tty);
	
	tty = old_tty;

	tty.c_lflag &= ~(ICANON | ECHO | ISIG);

	//убираем вывод new line при неканоническом выводе ECHONL
	//пока ни на что не влияет
	//tty.c_lflag &= ~(ECHONL);

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//нужно обработать перемещение стрелок
	//выход просто по CTRL_D


	const int CTRL_W = 0x17, CTRL_D = 4, KILL = 3, ERASE = 0x7F;

	tcsetattr(STDIN, TCSANOW, &tty); 

	char c, buf[3], sound = 7, nl = '\n', test = 'P';
	int s_in_line = 0, curr_line = 0;
	//char line[MAX_LINE] ={0};
	char* lines[100];
	lines[0] = calloc(MAX_LINE + 3, 1);

	while(1){
		int res = read(STDIN, buf, 3);
		c = buf[0];
		if(res == 0){
			break;
		}else if(res > 0){
				
			if(c == CTRL_W){
				s_in_line = eraseWord(STDIN, lines[curr_line], s_in_line);
			}else if(c == CTRL_D){
				write(1,&nl,1);
				break;
			}else if(c == KILL){
				s_in_line = eraseLine(STDIN, lines[curr_line], s_in_line);
			}else if(c == ERASE){
				s_in_line = eraseSimbol(STDIN, lines[curr_line], s_in_line);
			}else if(iscntrl(c)){
				write(1,&sound,1);
				continue;
			}else{	
				memcpy(lines[curr_line] + s_in_line, buf, res);
				//lines[curr_line][s_in_line] = c;
				s_in_line+=res;
				write(1, &buf, res);
				if(c == nl) s_in_line = 0;
				if(s_in_line >= MAX_LINE){
					write(1,&nl,1);
					lines[++curr_line] = calloc(MAX_LINE + 3,1);
					s_in_line = 0;
				}
			}

		}else{
			break;
			//perror(0);
		}
	}

	tcsetattr(STDIN, TCSANOW, &old_tty); 

	return 0;
}