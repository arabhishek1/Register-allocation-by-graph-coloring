
#include<stdio.h>
#include<stdlib.h>
#include "impl.h"
_Bool assign_colours(int noOfVar, char *filename, set_t* s)

{
	_Bool sufficient = 1;
	list_t l;
	init(&l);
	static int reg = 0;
	int graphMatrix[noOfVar][noOfVar];
	int copyGraph[noOfVar][noOfVar];
	read_from_file(graphMatrix,noOfVar,"RIG.txt");
	copy_matrix(copyGraph,graphMatrix,noOfVar);
       
	degreeNode(&l,noOfVar,copyGraph);

	printf("\n ---Variables of expression along with their liveness---------- \n");
	displayList(&l,s);
	if(reg == 0)
	{
		printf("\n Enter the no of reg : ");
		scanf("%d",&reg);
	}
	
	int stack[noOfVar];
	int arr[noOfVar];
	int stkptr=-1;
	initStack(stack,noOfVar);

	//im initially initializing the stack elemenoOfVarts with -1 to indicate its empty
	
	stkptr=stackPush(&l,reg,stack,noOfVar,stkptr,copyGraph,s);
	//The if condn is used to check whether thr r sufficient no of reg to allocate to variables
	//im comparing it with stkptr bcoz if thr r insufficient reg thn the elements/nodes would not be pushed into stack
	if(stkptr==noOfVar-1)
	{
	stackPop(stack,noOfVar,stkptr,graphMatrix,reg,arr);
	displayReg(noOfVar,arr,s);
	}
	else 
	{
		printf("\n -------------- Insufficient Registers,Spilling is required....-------------\n\n\n\n");
		sufficient = 0;
	}
	return sufficient;
}


void init(list_t* ptr)
{
  ptr->head=0;
}

void add(list_t* l,int degree,int index)
{
  node_t* temp;
  temp=(node_t*)malloc(sizeof(node_t));
  temp->degree=degree;
  temp->index=index;
  
  node_t* pres = l->head;
  if(l->head==0)
  {
        l->head=temp;
        temp->link=0;
  }
  else if((temp->degree ) < (pres->degree))
  {
	temp->link=l->head;
	l->head=temp;
  }
  else if(temp->degree >= pres->degree)
  {
	node_t* prev=0;
	while(pres && ((temp->degree) >= (pres->degree)))
	{
	  prev=pres;
	  pres=pres->link;
	}
	prev->link= temp;
	temp->link=pres;
  }
  
  
}


void displayList(list_t* ptr, set_t* s)
{
	 node_t *temp;
	 temp=ptr->head;
	 printf("\n");
	 while(temp)
	 {
		printf("\t  %s -> %d",getVarFromIndex(temp->index,s),temp->degree);
		temp=temp->link;
	 
	 }

	printf("\n");
}
void displayMatrix(int noOfVar,int graphMatrix[][noOfVar])
{
	int i,j;
	for(i=0;i<noOfVar;i++)
        {
                for(j=0;j<noOfVar;j++)
                {
                        printf(" %d  ",graphMatrix[i][j]);
                }
                printf("\n");
        }
}
void displayArray(int a[],int noOfVar,int stkptr, set_t* s)
{
	int i;
	
	for(i=noOfVar-1;i>=0;i--)
	{
	  if(a[i]>-1)
	  {
		printf(" %s ",getVarFromIndex(i,s));
	
	  }
	  
	}
	if(stkptr==-1)
	{
	  printf("\n--- Stack is empty.Nothing to display!!!!! ----\n");
	}
}

void degreeNode(list_t* ptr,int noOfVar,int copyGraph[][noOfVar])
{
	int i,j;
	for(i=0;i<noOfVar;i++)
        {
            int sum=0;
            for(j=0;j<noOfVar;j++)
            {
		sum+=copyGraph[i][j];
		
	    }
	    add(ptr,sum,i);
        }
	

}

void initStack(int stack[],int noOfVar)
{
	int i;
	for(i=0;i<noOfVar;i++)
	{
		stack[i]=(-1);
	} 
}

void changeDegree(list_t* ptr,int noOfVar,int copyGraph[][noOfVar])
{
  //calculates the degree of nodes
	int i,j;
	for(i=0;i<noOfVar;i++)
        {
            int degreeNode=0;
            for(j=0;j<noOfVar;j++)
            {
		degreeNode+=copyGraph[i][j];
		
	    }
	    changeKey(ptr,degreeNode,i);
        }
	

}

void changeKey(list_t* ptr,int degree,int i)
{
  node_t* temp;
  temp=ptr->head;
  while(temp)
  {
    if(temp->index==i)
    {
      temp->degree=degree;
    }
    temp=temp->link;
  }
  
}

void stackPop(int stack[],int noOfVar,int stkptr,int graphMatrix[][noOfVar],int reg,int arr[])
{
    /*char registers[4];
    registers[0]='R';
    registers[1]='G';
    registers[2]='B';
    registers[3]='Y';*/
    
    
    int i,index,count=0;
    int registers[reg];
    for(i=0;i<reg;i++)
    {
      registers[i]=i;
    }
    /*
     In this function i'm not literally poping the stack elements i'm just scanning stack and assigning colors to stack elements based on the order in which they are pushed
     I have used char array to represent reg if it can represented in a better way thn inform me
     */
    for(i=stkptr; i>=0; i--)
    {
      index=stack[i];
      if(count<reg)
      { 
	//in this block ill assign all the available reg to each elements sequentially based on the order in which theuy are pushed to stack 
	arr[index]=registers[count];
	++count;
      }
      else
      {
	//here i'll scan through the graph to check whether an edge exits between the nodes thn assign previously allocated reg if their aint any edge
	int counter=stkptr;
	while(graphMatrix[index][stack[counter]] !=0  && (index != stack[counter]))
	{
	  --counter;
	}
	arr[index]=arr[stack[counter]];
      
      }
    
    }
  
  
}

void displayReg(int noOfVar,int arr[],set_t* s)
{
  int i;
  printf("\n\n------------The REG allocated are ----------- \n\n");
//im just displayReg itself which are integers but i hve done char manipulation to show the graph n nodes neatly as suggested by Abdus
  for (i=0;i<noOfVar;i++)
  {
    printf(" %s:%d \t ",getVarFromIndex(i,s),arr[i]);
  }
  printf("\n\n\n");
}

int stackPush(list_t* ptr,int reg,int stack[],int noOfVar,int stkptr,int copyGraph[][noOfVar], set_t* s)
{
  //in this function im pushing the index of nodes into stack
  //and after pushing the index of node into stack im deleting all the edges associated with tht node
  //after deleting the degree of nodes are calculated once again.
	int i,j;
	node_t* temp;
	temp=ptr->head;
	while(temp)
	{
	  //the if condn checks whether the degree of node is less thn the available no of registers
	    if(temp->degree < reg)
	    {
	      
	      stack[++stkptr]=temp->index;
	      
	      printf("\nStack while  pushing \n");
	      displayArray(stack,noOfVar,stkptr, s);
	      printf("\n");
	      
	      for(i=0;i<noOfVar;i++)
	      {
		//here im deleting the edges by assigning zero
		copyGraph[i][temp->index]=0;
		for(j=0;j<noOfVar;j++)
		{
		  copyGraph[temp->index][j]=0;
		}
		
	      }
	      
	    }
	  
	  temp=temp->link;
	  //after deleting the node im recalculating the degree by calling changeDegree function
	  changeDegree(ptr,noOfVar,copyGraph);
	  
	  
	}
	printf("\n--------------- Graph aftr pushing:-------------- \n");
        displayMatrix(noOfVar,copyGraph);
	//degreeNode(b,n,a);
	printf("\n ----------Stack after pushing------------------ \n\n\n");
	displayArray(stack,noOfVar,stkptr,s);
	
	printf("\n\n\n\nThe value of stack pointer is %d \n",stkptr);
	
	return stkptr;

}

void read_from_file(int noOfVar; int graphMatrix[][noOfVar], int noOfVar, char *filename)
{
	FILE *fp;
	fp=fopen(filename,"r");
	int i,j;
	for(i=0;i<noOfVar;i++)
		{
		        for(j=0;j<noOfVar;j++)
		        {
		                fscanf(fp,"%d",&graphMatrix[i][j]);
		        }
		}
	fclose(fp);
}
void copy_matrix(int noOfVar; int copyGraph[][noOfVar],int graphMatrix[][noOfVar],int noOfVar)
{
	int i,j;
	 for(i=0;i<noOfVar;i++)
        {
                for(j=0;j<noOfVar;j++)
                {
                        copyGraph[i][j]=graphMatrix[i][j];
                }
        }
        

}

	
