#include <sys/ioctl.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>


//Done

typedef struct node{
	char* data;
	struct node* next;
}Node;


Node* push(Node* tail , Node* node){
	node->next = NULL;
	if(NULL != tail){
		tail->next = node;
	}
	return node;
}

void init(Node* head){
	while(head != NULL){
		printf("%s\n", head->data);
		Node* nd = head;
		head = head->next;
		free(nd);
	}
}


char buffer[998001];//получить размер буффера


int main(){
	Node* head = NULL , *tail = NULL;
	//struct winsize w;
   // ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
   // char* buffer = (char*)malloc(w.ws_row * w.ws_col);
	while(gets(buffer) != NULL){
		if(buffer[0] == '.'){
			init(head); return 0;
		}
		Node* nd = (Node*)malloc(sizeof(Node));
		if(NULL == head) head = nd;
		int slen = (strlen(buffer) + 1);
		nd->data = (char*)malloc(sizeof(char) * slen);
		memcpy(nd->data , buffer , slen);
		tail = push(tail , nd);
	}


	init(head);
	return 0;
}


