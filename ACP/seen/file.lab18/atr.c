
//189 стр Иртегов
//man

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include<string.h>
//https://www.ibm.com/developerworks/ru/library/l-lpic1-v3-104-6/index.html
//ls -i , ln foo bar, ln -s foo bar
//https://docs.altlinux.org/ru-RU/archive/2.4/html-single/master/alt-docs-extras-linuxcovice/ch02s09.html
//https://losst.ru/simvolicheskie-i-zhestkie-ssylki-linux
//https://www.ibm.com/developerworks/ru/library/au-unix-readdir/index.html
void showAttr(mode_t mode, int r, int w, int e){
	char c;
	c = mode & r ? 'r' : '-';
	printf("%c", c);
	c = mode & w ? 'w' : '-';
	printf("%c", c);
	c = mode & e ? 'x' : '-';
	printf("%c", c);
}

int main(int argc, char const *argv[])
{
	if(argc < 2 ){
		fprintf(stderr,"%s\n", "Path required");
		return 0;
	}

	const char* path = argv[1];

	struct stat info;
	//ссылки файлы
	if(lstat(path, &info) == -1){
		perror(0);
	}

	if(S_ISDIR(info.st_mode)){
		printf("%c", 'd');
	}else if(S_ISREG(info.st_mode)){
		printf("%c", '-');
	}else{
		printf("%c", '?');
	}

	showAttr(info.st_mode, S_IRUSR, S_IWUSR, S_IXUSR);
	showAttr(info.st_mode, S_IRGRP, S_IWGRP, S_IXGRP);
	showAttr(info.st_mode, S_IROTH, S_IWOTH, S_IXOTH);

	struct passwd* p = getpwuid(info.st_uid);
	struct group* gr = getgrgid(info.st_gid);

	printf("%14s", p->pw_name);
	printf("%14s", gr->gr_name);

	/*if(S_ISREG(info.st_mode)){
		printf("%7ld", info.st_size);
	}else{
		printf("      -");
	}*/
	printf("%7ld", info.st_size);

	char* mt = ctime(&info.st_mtime);
	int tr = strlen(mt) - 1;
	mt[tr] = '\0';//убираем \n,после tr есть уже '\0'
	printf("%30s ", mt);

	int i = strlen(path);
	while(i != -1){
		if(path[i] == '/'){
			break;
		}
		--i;
	}
	printf("%10s\n", &path[i + 1]);
	
	return 0;
}