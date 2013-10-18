//This file contains all implementation of spilling functions
#include<string.h>
#include<stdlib.h>
#include"spilling.h"
#include"CFG.h"


void insert_quadruple_for_load(basic_block_t* ptr_basic_block, quadruple_t *quad, char str[50])
{
		char var[50];
		char store[20]="LOAD ";
		strcpy(var, strcat(store, str));
		
		quadruple_t* temp = make_quadruple( var );
		
		quadruple_t* pres = ptr_basic_block->head;
		
		if(pres == quad)	//add in the begining...
		{
			temp->link=pres;
			ptr_basic_block->head=temp;
		}
		
		else	//add in the middle or end...
		{
		
			while(pres -> link != quad)
			{
				
				pres = pres->link;
			}
			
			temp->link=pres->link;
			pres->link =temp;
		
		}
}

void load_quad(basic_block_t* block ,quadruple_t *quad, char var[50])
{
		
		_Bool check=0;
		char ch;
		
		char dummy_inst[50];
		strcpy(dummy_inst, quad->inst);
		char *tok1 = strtok( dummy_inst , " =-+");
		
		char* tok2 = strtok( NULL , " =-+*/");
		char *tok3 = strtok(NULL , "+-*/ ");
		
		if(!( isFirstTokenStore( tok1 ) && isFirstTokenLoad( tok1 )))
		{
			if(strcmp(tok2, var) == 0)
			{
				check=1;
				insert_quadruple_for_load(block,quad,var);
			}
			
		
			if(strcmp(tok3, var) == 0)
			{
				if(! check )
				{
					insert_quadruple_for_load(block,quad,var);
				}
			}
		}
		else if(isFirstTokenStore(tok1))
		{
			if(strcmp(tok2, var) == 0)
			{
				insert_quadruple_for_load(block,quad,var);
			}
		}
}

void insert_quadruple_for_store(basic_block_t* ptr_basic_block, quadruple_t *quad, char str[50])
{
	
    // make a quadruple
	// fillup the quadruple
	//copy the char array; ground the pointer
		char var[50];
		char store[20]="STORE ";
		strcpy(var,strcat(store, str));
		
		quadruple_t* temp = make_quadruple( var);
	
		quadruple_t* pres = ptr_basic_block->head;
		
		while(pres != quad)
		{
			
			pres = pres->link;
		}
		
		temp->link =pres->link ;
		pres->link =temp;
}

void store_quad(basic_block_t* block ,quadruple_t *quad, char var[50])
{
		char* def = find_definition( quad->inst );
		if(strcmp(def, var) == 0)
		{
			insert_quadruple_for_store(block,quad,var);
		}
	
}


void removeUsagesAndDefinitions_quad(basic_block_t* block , quadruple_t* q,  char *var)
{
	
	//for a particular quadruple
	//	get the set.
	
	set_t * ptr_set = &q->live;
	removeFromSet(ptr_set, var);	//removing the usages...
	if(strcmp(var, q->def)==0)
	{
		strcpy(q->def, "");
	}
}

void removeAllUsagesAndDefinitions_forEveryBlocks(basic_block_t* block , char *var)
{
	quadruple_t* q= block->head;
	while(q)
	{
		removeUsagesAndDefinitions_quad(block,q,var);
		q = q->link;
	}
	
}

void removeAllUsagesAndDefinitions(basic_block_t blocks[], int num_of_blocks, char *var)
{
	//int z;
		//	scanf("%d",&z);
			//printf("\n input : %d",z);
	int i=0;
	for(i=0; i<num_of_blocks; i++)
	{
		removeFromSet(&blocks[i].entry, var);
		removeFromSet(&blocks[i].exit, var);

		removeAllUsagesAndDefinitions_forEveryBlocks(&blocks[i],var); 
	}
}

//free the memory allocated for the instruction

char* getLoadInstruction( char* var )
{
	char * inst = malloc( sizeof( char )*25 );

	//this memory has to be freed after its use
	strcpy( inst , "LOAD ");
	strcpy( inst + 5 , var );
	return inst;
}

char* getStoreInstruction( char * var)
{
	char * inst = malloc( sizeof( char )*50 );

	strcpy( inst , "STORE ");
	strcpy( inst + 6 , var );
	return inst;
}

void freeInstruction( char* inst)
{
	free( inst );
}

char* get_var_for_spilling(int num_of_var; int graph[][num_of_var] , int num_of_var , set_t* all_var_set )
{
	
	int index = get_max_degree(graph,num_of_var);	//why 5????
	return getVarFromIndex(index , all_var_set);
}

int get_max_degree(int n; int mat[n][n], int n)
{
	int index=0;
	int var=0;
	int i,j;
	for(i=0; i<n; ++i)
	{
		int count=0;
		for(j=0; j<n; ++j)
		{
			if(mat[i][j])
			{
				count++;
			}
		}
		if(count > var)
		{
			var=count;
			index=i;
		}
	}
	return index;
}

void str_rev(char * temp)
{
	int last = strlen( temp ) - 1;
	int i = 0;
	while( last > i )
	{
		char t = temp[last];
		temp[last] = temp[i];
		temp[i] = t;
		--last;++i;
	}
}

char* getTheNewVar( char* var , int i )
{
	char* new_var = (char*)malloc(sizeof( char )*50);
	strcpy( new_var , var );
	char temp[10] = "";

	int counter = 0;
	while( i )
	{
		
		temp[counter] = (i%10) + '0';
		i = i/10;
		++counter;
	}
	temp[counter] = '\0';
	str_rev(temp);

	strcat( new_var , temp );
	return new_var;
}

void shift_left(char str[], int n)
{
	int len=strlen(str);
	if(n <= len)
	{
		int i=0;
		while(n <= len )
		{
			str[i]=str[n];
			n=n+1;
			i=i+1;
		}
	}
	else
	{
		strcpy(str, "");
	}

	//printf("%s\n",str);
	
}

int getIndex( char inst[] , char oldVar[], int token_length)
{
	int start_index = token_length;
	int varlength = strlen(oldVar);
	int instlength = strlen(inst);
	int index = -1;

	printf("inst - var : %d\n",instlength-varlength);

	int i;
	for( i = start_index ; i <= instlength-varlength;++i)
	{
		int j;
		_Bool matched = 1;
		for(j = i ; j < varlength +i ; ++j)
		{
			matched = matched && (inst[j] == oldVar[j-i]);
		}
		if( matched )
		{
			index = i;
			return index;
		}
	}

	if(index < 0 ) printf("The var is not found in the instruction...\n");
	return index;
}

_Bool changeInstruction(char inst[], char oldVar[], char newVar[])
{

	//here is the error..........
	char temp[50];
	strcpy( temp , inst );
	//Here if we get the required variable, we return true else we return flase
	_Bool var_obtained = 0;
	
	
	char *tok1 = strtok( temp , " =-+*");
	char *tok2 = strtok( NULL , " =-+*");
	char *tok3 = strtok( NULL , " =-+*");

	if(strcmp(oldVar,tok1)==0)
	{
		char inst_temp[50];

		strcpy(inst_temp, inst);
		
		shift_left(inst_temp, strlen(tok1));
		inst[0]='\0';
		strcpy(inst,newVar);
		strcat(inst, inst_temp);
		var_obtained  = 1;
	}

	if(strcmp(oldVar,tok2)==0)
	{
		int index = getIndex( inst , oldVar , strlen(tok1));
		printf("index : %d\n",index);
		if(index >= 0)
		{
			char* array = inst + index;
			char inst_temp[50];

			printf("intruction now going to be spilled : %s",inst);
			strcpy( inst_temp , array);
			shift_left( inst_temp , strlen(tok2));
			array[0]='\0';
			strcat(array , newVar);
			strcat( array , inst_temp);
			
			printf("\nIntruction after spilling : %s\n",inst);
		}
		var_obtained  = 1;
	}

	if(tok3 != NULL && (strcmp(oldVar,tok3)==0))
	{
		int tokenLen = strlen(tok3);
		int instLen = strlen(inst);
		inst[instLen-tokenLen]='\0';
		strcat(inst, newVar);
		var_obtained  = 1;
	}
	return var_obtained;
}

void addInstructionforQuad( basic_block_t *ptr_block , quadruple_t *ptr_quad , char* var, _Bool var_changed)
{
	static int i = 1;

	if( var_changed )	//something to 
	{
		i = 1;
	}

	//i.e for every instruction
	//	get the new variable
	//	change the instruction
	//	add the additional instruction to the block

	char* newVar = getTheNewVar( var , i );
	_Bool var_obtained = changeInstruction( ptr_quad->inst , var , newVar );
	printf("\nvar obtained : %d",var_obtained);
	if( var_obtained )
	{
		store_quad( ptr_block , ptr_quad , newVar);
		printf("\nvar obtained : %d",var_obtained);
		load_quad( ptr_block , ptr_quad , newVar);
		printf("After store load\n");
	//	int z;
	//	scanf("%d",&z);
	//	printf("\ninput : %d",z);
		++i;
		printf("\nvar obtained : %d",var_obtained);
	}	
}


void addInstructions( basic_block_t* ptr_block , char *var , _Bool var_changed)
{
	quadruple_t * ptr_quad = ptr_block -> head;
	while( ptr_quad )
	{
		addInstructionforQuad( ptr_block , ptr_quad , var , var_changed);
		if( var_changed )  var_changed = !var_changed;
		ptr_quad = ptr_quad -> link;
	}
}

void spillVariable_s( char* var , basic_block_t blocks[] , int num_blocks , _Bool var_changed )
{
	//	remove all its usages and definitions
//	for every occurence of the variable
//	add the store or load instruction

	removeAllUsagesAndDefinitions( blocks , num_blocks , var);
	int i;

	//for every blocks add instructions
 
	for(i = 0 ; i < num_blocks ; ++i)
	{
		addInstructions( &blocks[i] , var , var_changed);
		if( var_changed ) var_changed = !var_changed;
	}
}

void spillVariable( basic_block_t blocks[] , char* var  , int num_blocks )
{
	spillVariable_s( var , blocks , num_blocks , 1);

}
