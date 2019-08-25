
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include<string.h>

/*$ echo hello, world > myfile  Создать файл 

$ ls -l myfile  Отобразить права доступа 

-rw-r--r-- 1 arnold devel 13 Apr 3 17:11 myfile

$ chmod g+s myfile  Добавить бит setgid

$ ls -l myfile Показать новые права доступа 

-rw-r-Sr-- 1 arnold devel 13 Apr 3 17:11 myfile*/

/*Чтобы установить обязательный захват, файлу необходимо установить бит setgid, но
при этом файл не должен быть доступен по исполнению для группы. Все захваты,
устанавливаемые на такой файл, будут работать в обязательном режиме.*/

//Mandotory locking https://gavv.github.io/articles/file-locks/#mandatory-locking
//http://manpages.ubuntu.com/manpages/precise/ru/man2/fcntl.2.html
//advisory lock
int main(int argc, char const *argv[])
{
	if(argc != 2){
		printf("Which file?\n");
		return 0;
	}
	int fd = 0;
	if(-1 == (fd = open(argv[1] ,  O_RDONLY))){
		perror(argv[1]);
		exit(2);
	}
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;//lock entire file
	fcntl(fd, F_SETLK, &lock);

	char* sysc = "nano ";
	char* cmd = (char*)malloc(strlen(sysc) + strlen(argv[1]));
	memcpy(cmd , sysc , strlen(sysc));
	memcpy(cmd + strlen(sysc) , argv[1] , strlen(argv[1]));
	system(cmd);
	return 0;
}


//setuid
//https://ru.wikipedia.org/wiki/Suid

 /*предлагаемые разными интерфейсами блокировки на уровне системы
   (flock, lockf, fcntl) являются "совещательными" (advisory),*/
/*Вспомогательная блокировка

Вспомогательная блокировка (advisory lock) тесно соответствует замку на двери. Говорят, «замки существуют для
 честных людей», что означает, что если кто-нибудь на самом деле захочет вломиться в ваш дом, он, возможно, 
 найдет способ это сделать, несмотря на наличие замка в двери. То же и со вспомогательной блокировкой; она работает
  лишь тогда, когда тот, кто пытается получить доступ к заблокированному файлу, сначала пытается получить блокировку.
   Однако, программа может совершенно игнорировать вспомогательные блокировки и делать с файлом, что захочет
    (конечно, пока это разрешается правами допуска файла).

Обязательная блокировка

Обязательная блокировка является более строгой: когда установлена обязательная блокировка,
 ни один другой процесс не может получить доступ к заблокированному файлу. 
 Любой процесс, который пытается игнорировать это, либо сам блокируется до снятия блокировки файла, 
 либо его попытка завершится неудачей. (Под GNU/Linux по крайней мере это включает*/