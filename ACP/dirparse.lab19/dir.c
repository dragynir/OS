
#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>

extern errno;


/*╥        * соответствует последовательности любых символов кроме /, имеющей любую длину; возможно - пустой последовательности.

╥        ? соответствует любому одному символу.

╥        / не может встречаться.

╥        любой другой символ соответствует самому себе*/

int showOrNot(const char* pattern, char* name){

	int n_len = strlen(name);
	int p_len = strlen(pattern);
	int curr_i = 0, matches = 2;
	for (int i = 0; i < p_len; ++i)
	{
		if(curr_i >= n_len){
		 	 while(i < p_len){
		 	 	if(pattern[i] != '*') return 1;//not ok
		 	 	++i;
		 	 }
			 return 0;//ok
		}

		char cmd = pattern[i];

		if(cmd == '*'){
			int next_f = 0, r_count = 0, out_of_p = 0;
			while(1){
				++i;
				if(i >= p_len){
					out_of_p = 1;
					//printf("OUT, [%s]\n", name);
					break;
				}
				if(pattern[i] != '*' && pattern[i] != '?'){
					next_f = pattern[i];
					break;
				}else if(pattern[i] == '?'){
					++r_count;
				}
			}
			if(out_of_p == 1 && r_count == 0){
				return 0;//ok
			}else if(r_count != 0 && next_f == 0){
				if(r_count <= n_len - curr_i) return 0;
			}

			int contain_r = 1;//содержит нужное кол-во символов
			while(1){
				if(curr_i >= n_len){
					return 1;//not ok
				}
				if(name[curr_i] == next_f){
					if(r_count != 0){
						return 1;//not ok	
					}
					break;	
				}
				--r_count;
				++curr_i;
			}
			++i;
			++curr_i;
		}else if(cmd == '?'){
			++curr_i;
		}else if(cmd == '/'){
			printf("Pattern can't contain / .\n");
			return 1;
		}else{
			if(name[curr_i] != pattern[curr_i]){
				return 1;//not ok
			}else{
				++curr_i;
			}
		}
	}
	if(curr_i < n_len) return 1;
	return 0;
}


int main(int argc, char const *argv[])
{


	if(argc != 2){
		printf("Only pattern required\n");
		return 1;
	}

	const char* pattern = argv[1];


	DIR* c_dir = opendir(".");

	if(NULL == c_dir){
		printf("Directory not exist.\n");
	}


	struct dirent* file;
	int total_c = 0;
	while(NULL != (file = readdir(c_dir))){
		if(!showOrNot(pattern, file->d_name)){
			printf("%s\n", file->d_name);
			++total_c;
		}
	}
	if(errno){
		perror("Something goes wrong.\n");
		return 1;
	}
	if(total_c == 0){
		printf("Your pattern: %s\n",pattern);
	}
	return 0;
}