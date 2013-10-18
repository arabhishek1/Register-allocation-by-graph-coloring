#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#ifndef set
#define set set

struct node_set
{
	char variable[15];				//variable name.
	struct node_set* link;									
};
typedef struct node_set node_set_t;

struct set
{
	node_set_t* head;
};
typedef struct set set_t;

void initSet(set_t* s);
void addElements(set_t* s,char ch[]);				//adding variables to the set.
set_t doUnion(const set_t* s1, const set_t* s2);		//doing union of two sets.
void display(const set_t* s);
char* getVarFromIndex(int index, set_t* s);
set_t copy(set_t *s);
void deallocate(set_t* s);
void removeFromSet(set_t* s, char* str);



#endif
