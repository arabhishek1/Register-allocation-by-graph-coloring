#include"set.h"
#ifndef REG
#define REG

struct node
{
	int degree;
	int index;
	struct node* link;
};

typedef struct node node_t;

struct list
{
	node_t* head;
};	

typedef struct list list_t;

//I have used linked list to store both the degree and index of node .I'm storing index bcoz the i need the an order to push the nodes into stack; 

void init(list_t* ptr);
void displayList(list_t* ptr,set_t* s);
void add(list_t* ptr,int degree,int index);
void displayMatrix(int noOfVar,int graphMatrix[][noOfVar]);
void initStack(int a[],int noOfVar);
void displayArray(int a[],int noOfVar,int stkptr,set_t* s);
void degreeNode(list_t* ptr,int noOfVar,int a[][noOfVar]);
void changeKey(list_t* ptr,int degree,int i);
void changeDegree(list_t* ptr,int noOfVar,int copyGraph[][noOfVar]);
void stackPop(int stack[],int noOfVar,int stkptr,int graphMatrix[][noOfVar],int reg,int arr[]);
void displayReg(int noOfVar,int arr[],set_t* s);
int stackPush(list_t* ptr,int reg,int stack[],int noOfVar,int stkptr,int copyGraph[][noOfVar], set_t* s);

//***********************************************************************************************************
void write_matrix_into_file(int num_of_var; int displayMatrix[][num_of_var] , int num_of_var, char* filename);
void init_matrix(int num_of_var ; int graph[][num_of_var], int num_of_var);
_Bool assign_colours(int noOfVar, char *filename, set_t* s);
void read_from_file(int n; int graphMatrix[][n], int n, char *filename);
void copy_matrix(int noOfVar; int copyGraph[][noOfVar],int graphMatrix[][noOfVar],int noOfVar);

#endif
