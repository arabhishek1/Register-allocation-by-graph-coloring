#include<stdio.h>
#include"impl.h"
int main()
{
  
	list_t l;
	init(&l);
	int i,j,reg;
	int graphMatrix[noOfVar][noOfVar];
	int copyGraph[noOfVar][noOfVar];
	char name[10];
	// if thr's a better way to store any of the things,graph,index,registers,thn inform ill implement in tht manner
	printf("\nEnter the file name to read the graph\n");
	scanf("%s",name);
	FILE *fp;
	fp=fopen(name,"r");
	while(!feof(fp))
	{
        for(i=0;i<noOfVar;i++)
        {
                for(j=0;j<noOfVar;j++)
                {
                        fscanf(fp,"%d",&graphMatrix[i][j]);
                }
        }
	}
        
        for(i=0;i<noOfVar;i++)
        {
                for(j=0;j<noOfVar;j++)
                {
                        copyGraph[i][j]=graphMatrix[i][j];
			//here im copying the graph into another 2d matrix bcoz aftr pushing the nodes into stack the original graph will be changed so i need a copy 
			//to assign colors/reg based on whether an edge exists r not
                }
        }
        
        printf("\n ----------The Entered graph ----------\n");
	displayMatrix(noOfVar,graphMatrix);
	degreeNode(&l,noOfVar,copyGraph);
	printf("\n ---Nodes of graph along with their degrees \n");
	displayList(&l);
	 //im sorting the list based on the degree of nodes bcoz it ll make pushing the nodes into stack easy.
	//sort(&l);
	printf("\n----- Nodes  After sorting based on degree of nodes ------\n");
	displayList(&l);
	
	printf("\n Enter the no of reg : ");
	scanf("%d",&reg);
	
	int stack[noOfVar];
	char arr[noOfVar];
	int stkptr=-1;
	initStack(stack,noOfVar);
	//im initially initializing the stack elemenoOfVarts with -1 to indicate its empty
	
	stkptr=stackPush(&l,reg,stack,noOfVar,stkptr,copyGraph);
	//The if condn is used to check whether thr r sufficient no of reg to allocate to variables
	//im comparing it with stkptr bcoz if thr r insufficient reg thn the elements/nodes would not be pushed into stack
	if(stkptr==noOfVar-1)
	{
	stackPop(stack,noOfVar,stkptr,graphMatrix,reg,arr);
	displayReg(noOfVar,arr);
	}
	else printf("\n -------------- Insufficient Registers-------------\n\n\n\n");	

	return 0;
}
