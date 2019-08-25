#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include<ulimit.h>
#include <sys/time.h>
#include <sys/resource.h>
#include<string.h>

 extern char **environ;

 int parse_arg(char* arg , long int* val){
		if(arg == 0) return 0;
		char endptrd = 'c';
		char* endptrp = &endptrd;
		char** endptr =  &endptrp;
		long int new_v = strtol(arg , endptr, 10);
		if(**endptr == '\0'){
			*val = new_v;
			return 1;
		}else{
			return 0;
		}
 }


int set_resourse(int* r , char s){
	int resource = 0;

	switch(s)
			{
		case 'c':
		  resource = RLIMIT_CORE; 
		  break;
	    case 'd':
		  resource = RLIMIT_DATA; 
		  break;
		case 'f':
		  resource = RLIMIT_FSIZE; 
		  break;
		case 'i':
		  resource = RLIMIT_SIGPENDING; 
		  break;
		case 'l':
		  resource = RLIMIT_MEMLOCK; 
		  break;
		case 'm':
		  resource = RLIMIT_RSS; 
		  break;
		case 'n':
		  resource = RLIMIT_NOFILE; 
		  break;
		case 'q':
		  resource = RLIMIT_MSGQUEUE; 
		  break;
		case 'r':
		  resource = RLIMIT_RTPRIO; 
		  break;
		case 's':
		  resource = RLIMIT_STACK; 
		  break;
		case 't':
		  resource = RLIMIT_CPU; 
		  break;
		case 'u':
		  resource = RLIMIT_NPROC; 
		  break;
		case 'v':
		  resource = RLIMIT_AS; 
		  break;
		case 'x':
		  resource = RLIMIT_LOCKS; 
		  break;
		default: return 1; break;
    }
    *r = resource;
    return 0;
}


int interact(int argc , char* argv[] , char* options){
	int c = getopt(argc , argv , options);
	if(c == '?') return 1;
	char* opta = optarg;
	int err = 0;
	if(c != EOF){
		err = interact(argc , argv , options);
	}
	if(err) return err;

     struct rlimit rlim;
     long int size = 0 , new_ulimit;
     int resource = 0;
     char endptrd = 'c';
     char* endptrp = &endptrd;
     char** endptr =  &endptrp;

	 switch (c)
	  {
		case 'i':
		  printf("Real User id: %d\n" , getuid());
		  printf("Effective User id: %d\n" , geteuid());
		  break;
		case 's':
		  setpgid(0 , 0);
		  break;

		case 'p':
	      printf("Process ID: %d\n" , getpid());
	      printf("Parent process ID: %d\n" , getppid());
	      printf("Process group ID: %d\n" , getpgid(0));//0 чтобы вызвать у текущего процесса
		  break;

		case 'u':
		  if(0 == opta || strlen(opta) != 1){
		  	printf("Invalid arg to -u.\n");
		  	return 1;
		  }

		  if(0 != set_resourse(&resource , opta[0])){
				printf("Unknown limit.\n");
				return 1;
		  }
	      getrlimit(resource , &rlim);
	      printf("Ulimit: %ld\n" , rlim.rlim_max);
		  break;

		case 'U':	 
			if(0 == opta || strlen(opta) < 3 || opta[1] != '='){
				printf("Invalid arg to -U.\n");
				return 1;
			}

			if(!parse_arg(opta + 2 , &new_ulimit)){
				printf("Invalid arg to -U.\n");
				printf("%c= Expected value.\n" , opta[0]);
				return 1;
			}
		    rlim.rlim_max = new_ulimit;
			if(0 != set_resourse(&resource , opta[0])){
				printf("Unknown limit.\n");
				return 1;
			}
		    setrlimit(resource , &rlim);
		  break;

		case 'c':
			getrlimit(RLIMIT_CORE , &rlim);
			printf("Core file size: %ld\n" , rlim.rlim_cur);
		  break;

		case 'C':
			if(!parse_arg(opta , &size)){
			 	printf("Invalid arg to -C.\n");
			 	return 1;
			}
			rlim.rlim_cur = size;
			setrlimit(RLIMIT_CORE , &rlim);
		  break;

		case 'v':
			for (char** p = environ; *p; p++)
				printf ("%s\n\n", *p);
		  break;


		case 'V':
		  if(0 == opta){printf("Invalid arg to V\n");}
		  if(0 != putenv(opta)){
		  	printf("Invalid environment variable pattern.\n");
		  	return 1;
		  }
		  break;

		case 'h':
		  system("ulimit -a");
		  break;
	  }

	  return 0;
}



 int  main(int argc, char *argv[]){
 	 printf("%d   %d\n",  RLIMIT_CORE , RLIMIT_DATA);
 	 return 0;
	 char options[] = "hispu:U:cC:dvV:"; /* valid options */

	 int c, invalid = 0, dflg = 0, fflg = 0, gflg = 0;
	 char *f_ptr, *g_ptr;

	 if(interact(argc , argv , options)){
	 	printf("Try again\n");
	 }

 }


 /*ввода-вывода). 
Процесс может посылать сигналы только членам своей группы процессов (process group), 
состоящей из его прямого родителя (и других предков), братьев и сестер, а также детей (и прочих потомков).
  Процесс может также послать сигнал сразу всей своей группе за один системный вызов.*/