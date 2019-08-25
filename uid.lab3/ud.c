#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>


/*Реальный (RID) и эффективный (EUID) идентификаторы пользователя

Реальным идентификатором пользователя
 данного процесса является идентификатор пользователя,
  запустившего процесс. Эффективный идентификатор
   служит для определения прав доступа процесса к
    системным ресурсам (в первую очередь к ресурсам
     файловой системы). Обычно реальный и эффективный
      идентификаторы эквивалентны, т.е. процесс имеет 
      в системе те же права, что и пользователь, запустивший его.
       Однако существует возможность задать
        процессу более широкие права,
         чем права пользователя путем
          установки флага SUID, когда
           эффективному идентификатору
            присваивается значение идентификатора 
            владельца исполняемого файла (например, администратора).*/


void test(){
	printf("Real User id: %d\n" , getuid());
    printf("Effective User id: %d\n" , geteuid());
	/*FILE* inf = fopen("f.txt" , "r");
	if(NULL == inf){ perror(NULL);return;}
	fclose(inf);*/
}
//Тут про setuid
//https://books.google.ru/books?id=OeVIDwAAQBAJ&pg=PA206&lpg=PA206&dq=%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%BD%D1%8B%D0%B9+%D0%B2%D1%8B%D0%B7%D0%BE%D0%B2+setuid&source=bl&ots=VZ4mLOhnYG&sig=ACfU3U0OFMu7JUiTUAKzVwHPYcnwRf4LXw&hl=ru&sa=X&ved=2ahUKEwiY58qAmvXhAhUmxaYKHVBdAa8Q6AEwCXoECAgQAQ#v=onepage&q=%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D0%BD%D1%8B%D0%B9%20%D0%B2%D1%8B%D0%B7%D0%BE%D0%B2%20setuid&f=false


//To do
//2
//3
//https://it.wikireading.ru/34351
//https://www.opennet.ru/man.shtml?topic=setuid&category=2&russian=0
//https://www.linux.org.ru/forum/general/10220194
//http://ru.manpages.org/setuid/2
int main(int argc, char const *argv[])
{


   uid_t ruid;
  uid_t euid;
  uid_t suid;
  getresuid(&ruid, &euid, &suid);

   printf( "Real:%4d  Effective:%4d  Saved:%4d\n",
           ruid, euid, suid);

	//test();
	printf("%d" , setuid(10));

  getresuid(&ruid, &euid, &suid);

   printf( "Real:%4d  Effective:%4d  Saved:%4d\n",
           ruid, euid, suid);
	/*test();*/

	return 0;
}

/*                       |      seteuid             |          setuid  
Cond 1. (euid == root) | set euid to any uid      | set all three uids to any uid  
Cond 2. (euid != root) | set euid to ruid or suid | set euid to ruid or suid */

//начало с root = 0
/*Итак, в коде есть 5 процессов setuid или seteuid, позвольте мне их классифицировать: 
1. seteuid (600): Cond 1, установите euid на 600 
2. setuid (1000): Cond 2, установите euid на 1000 
3. setuid (0): Cond 2, установите euid в 0 (suid) 
4. setuid (1000): Cond 1, установите все три идентификатора на 1000 
5. setuid (0): Cond 2, все три идентификатора не равны 0,
 поэтому не могут быть установлены в 0, сбой с ret = -1*/ 