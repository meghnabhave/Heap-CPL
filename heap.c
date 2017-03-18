#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 20
typedef struct list_tag{
	int start;
	int size;
	struct list_tag *next;
}List;

typedef struct Alist_tag{
	int start;
	int size;
	char name;
	struct Alist_tag *next;
}AList;

List* makeNode(int start, int size){
	List *nptr = (List*)malloc(sizeof(List));
	nptr->start = start;
	nptr->size=size;
	nptr->next = NULL;
	return nptr;
}

AList* AmakeNode(int start, int size,char name){
	AList *nptr = (AList*)malloc(sizeof(AList));
	nptr->start = start;
	nptr->size=size;
	nptr->name = name;
	nptr->next = NULL;
	return nptr;
}

List* makeFreeList(){
	List *freeList;
	freeList = makeNode(0,SIZE);
	return freeList;	
}

AList* makeAllocatedList(){
	AList *allocatedList;
	allocatedList = NULL;
}

List* insertF(List *lptr, List *nptr){
	if(lptr == NULL){
		lptr = nptr;
	}
	else{
		List *prev,*cur;
		cur = lptr;
		if(cur!=NULL && cur->start<nptr->start){
			prev = cur;
			cur = cur->next;
		}
		if(cur != lptr){

			nptr->next = cur;
			prev->next = nptr;
		}
		else{
			nptr->next = lptr;
			lptr = nptr;
		}
	}
	return lptr;
}

AList* insertA(AList *lptr, AList *nptr){
	if(lptr == NULL){
		lptr = nptr;
	}
	else{
		AList *prev,*cur;
		cur = lptr;
		if(cur!=NULL && cur->start<nptr->start){
			prev = cur;
			cur = cur->next;
		}
		if(cur != lptr){

			nptr->next = cur;
			prev->next = nptr;
		}
		else{
			nptr->next = lptr;
			lptr = nptr;
		}
	}
	return lptr;
}

List* deleteF(List *freeList,List *ptr){
	if(ptr != NULL){
		List *prev,*cur;
		cur = freeList;
		while(cur!=NULL && cur != ptr){
			prev = cur;
			cur = cur->next;
		}
		if(cur==ptr){
			if(cur != freeList){
				prev = cur->next;
			}
			else{
				freeList = cur->next;
			}
			free(ptr);
		}
	}
	return freeList;
}

AList* deleteA(AList *list,AList *ptr){
	{
		AList *prev,*cur;
		cur = list;
		while(cur!=NULL && cur != ptr){
			prev = cur;
			cur = cur->next;
		}
		if(cur==ptr){
			if(cur != list){
				prev = cur->next;
			}
			else{
				list = cur->next;
			}
			free(ptr);
		}
	}
	return list;
}

void freeListCompact(List **freeList){
	List *nptr = *freeList;
	while(nptr!=NULL && nptr->next != NULL){
		if(nptr->start + nptr->size +1 == (nptr->next)->start){
			nptr->size = nptr->size + (nptr->next)->size;
			*freeList = deleteF(*freeList,nptr->next);
		}
		nptr = nptr->next;
	}
}

void heap_malloc(char name,char d[],char heap[],List **freeList, AList **allocatedList){
	int size = strlen(d);
	List *nptr = *freeList;
	AList *ptr = NULL;
	while(nptr!=NULL && nptr->size<size){
		nptr = nptr->next;
	}
	if(nptr!=NULL && nptr->size>=size){
		ptr = AmakeNode(nptr->start,size,name);
		int i,j=0;
		for(i=nptr->start;i<nptr->start+size;i++){
			heap[i] = d[j]; j++;
		}	
		*allocatedList = insertA(*allocatedList,ptr);
		List *p;
		if(nptr->size >size){
			p = makeNode(nptr->start+size,nptr->size - size);
			*freeList = insertF(*freeList,p);
		}
		*freeList = deleteF(*freeList,nptr);
	}
	else{
		printf("\nNo space in heap.");
	}
	//freeListCompact(freeList);
}

void heap_free(char name,char heap[],List **freeList, AList **allocatedList){
	AList *nptr = *allocatedList;
	while(nptr != NULL && nptr->name != name){
		nptr = nptr->next;
	}
	if(nptr == NULL){
		printf("\nNot in heap.");	
	}
	else{
		List *ptr;
		ptr = makeNode(nptr->start,nptr->size);
		*freeList = insertF(*freeList,ptr);
		*allocatedList = deleteA(*allocatedList,nptr);
	}
	//freeListCompact(freeList);
}

void printA(AList *lptr){
	AList *ptr = lptr;
	while(ptr != NULL){
		printf("%d %d --> ",ptr->start,ptr->size);
		ptr= ptr->next;
	}
	printf("NULL\n");
}

void printF(List *lptr){
	List *ptr = lptr;
	while(ptr != NULL){
		printf("%d %d --> ",ptr->start,ptr->size);
		ptr= ptr->next;
	}
	printf("NULL\n");
}

void printVar(char name,char heap[],AList *allocatedList){
	AList *nptr = allocatedList;
	while(nptr != NULL && nptr->name != name){
		nptr = nptr->next;
	}
	if(nptr == NULL){
		printf("\nNot in heap.");	
	}
	else{
		int i;
		for(i=nptr->start;i<nptr->start+nptr->size;i++){
			printf("%c",heap[i]);
		}
	}
}

void compact(char heap[],List **freeList, AList **allocatedList){
	char newHeap[SIZE];
	AList *nptr = *allocatedList;
	int i,j=0;
	while(nptr != NULL){
		int start = nptr->start;
		int size = nptr->size;
		nptr->start = j;
		for(i=start;i<start+size;i++){
			newHeap[j] = heap[i]; j++;
		}
		nptr = nptr->next;
	}
	int sum = 0;
	List *ptr = *freeList;
	List *p;
	while(ptr!=NULL){
		sum = sum + ptr->size;
		p = ptr;
		ptr = ptr->next;
		free(p);
	}
	*freeList = makeNode(j,sum);
	for(i=0;i<j;i++){
		heap[i] = newHeap[i];
	}
	
}

void main(){
	char heap[SIZE];
	List *freeList = makeFreeList();
	AList *allocatedList = makeAllocatedList();
	int ans;
	char d;
	
	do{
		printf("\n1.Allocate\n2.Free\n3.Compact\n4.Print\n5.Exit\nAnswer: ");
		scanf("%d",&ans);

		switch(ans){
			case 1:	printf("Enter name of variable: ");
				scanf("%c",&d);
				scanf("%c",&d);
				printf("Enter data: ");
				char data[10];
				scanf("%s",data);
				heap_malloc(d,data,heap,&freeList,&allocatedList);
				printf("\nFreeList: ");
				printF(freeList);
				printf("\nAllocated List: ");
				printA(allocatedList);
				
				break;
			case 2: printf("Enter name of variable to be freed: ");
				scanf("%c",&d); scanf("%c",&d);
				heap_free(d,heap,&freeList,&allocatedList);
				printf("\nFreeList: ");
				printF(freeList);
				printf("\nAllocated List: ");
				printA(allocatedList);
				break;
			case 3: //compaction
				compact(heap,&freeList,&allocatedList);
				printf("\nFreeList: ");
				printF(freeList);
				printf("\nAllocated List: ");
				printA(allocatedList);
				break;
			case 4: //print a var
				printf("Enter variable: ");
				scanf("%c",&d); scanf("%c",&d);
				printVar(d,heap,allocatedList);
				break;
			case 5:
				break;
			default: printf("\nInvalid");		
		}
	
	}while(ans != 5);
	

	
}
