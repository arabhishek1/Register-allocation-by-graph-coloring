#include"CFG.h"
#include<string.h>
#include<ctype.h>
#include"set.h"
#include"impl.h"

int get_num_of_quadruples( basic_block_t *ptr_basic_block )
{
	quadruple_t *ptr_quad = ptr_basic_block->head;
	int n = 0;
	while( ptr_quad )
	{
		++n;
		ptr_quad = ptr_quad->link;
	}
	return n;
}

void reverse_basic_block( basic_block_t *ptr_basic_block)
{
	int n = get_num_of_quadruples( ptr_basic_block );
	if(n>1)
	{
		//reverse is only done when we have more than 1 quadraple
		quadruple_t *ptr_prev = 0;
		quadruple_t *ptr_mid = ptr_basic_block->head; //Head points to the linked list of quadraples
		quadruple_t *ptr_next = ptr_basic_block->head->link;
		while(ptr_mid)
		{
			ptr_mid->link = ptr_prev;
			ptr_prev = ptr_mid;
			ptr_mid = ptr_next;
			if( ptr_next ) ptr_next = ptr_next->link;
		}
		ptr_basic_block->head = ptr_prev;
	}
}

_Bool isFirstTokenLoad( char *str )
{
	char load[] = "LOAD";

	//to preserve the value of string
	char strDummy[50];
	strcpy( strDummy , str );

	//use strDummy for all the other manipulation.		
	char *temp = strtok(strDummy , " */+-");

	char *t = temp;
	while( *t != '\0')
	{
		*t = toupper(*t);
		t++;
	}

	return !strcmp( load , temp);
}

_Bool isFirstTokenStore( char *tok)
{
	char store[] = "STORE";
	char tokDummy[15];

	//for safety
	strcpy( tokDummy , tok );

	//converting to upper case for comparing
	char *ptr = tokDummy;
	while( *ptr != '\0')
	{
		*ptr = toupper( *ptr );
		++ptr;
	}

	return ( strcmp( tokDummy , store ) == 0 );
}



_Bool findEqualsInString(char a[])
{
	int n= strlen(a);
	int i=0;
	for(i=0;i<n;++i)
	{
		if(a[i] == '=')
		{
			return 1;
		}
	}
	return 0;
}

char* find_definition( const char* const str )
{
	//copy the string to a temporary for safety
	char* temp = (char*)malloc( sizeof(char) * 50);
	strcpy( temp , str );
	
	if( findEqualsInString( temp ) )
	{
		//put the first token as the def
		return strtok( temp , "= */+-");
	}
	else if( isFirstTokenLoad( temp )) //the first token is load
	{
		//put the second token as the definition.
		strtok( temp , "= */+-");
		return strtok( NULL , "= */+-");
	}
	strcpy(temp , "");
	return temp;
}

quadruple_t * make_quadruple( char str[50] )
{
	quadruple_t * temp = (quadruple_t*)malloc(sizeof(quadruple_t));
	strcpy(temp->inst,str);
	temp->link = 0;
	initSet( &temp->live );
	temp->def[0] = '\0';
	return (quadruple_t*)temp;
}

void add_at_end(basic_block_t* ptr_basic_block, char str[50])
{
    // make a quadruple
	// fillup the quadruple
	//copy the char array; ground the pointer

     quadruple_t* temp = make_quadruple( str );

	// go to the end and add
	// add in the beginning if nothing is there
	if(ptr_basic_block->head == 0)
	{
		ptr_basic_block->head = temp;
		temp->link = 0;
	}
	else        // for a non empty basic_block
	{
		quadruple_t* pres = ptr_basic_block->head;
		while(pres->link)
		{
			pres = pres->link;
		}
		pres->link = temp;
		temp->link = 0;
	}
}

void set_definition_of_block( basic_block_t *ptr_block)
{
	quadruple_t * head = ptr_block->head;
	while(head)
	{
		strcpy(  head->def , find_definition( head->inst ) );
	  	head = head->link;
	}
}

void set_all_definitions( basic_block_t blocks[] , int n )
{
	int i = 0;
	for( i = 0 ; i < n ; ++i)
	{
		set_definition_of_block(&blocks[i]);
	}
}


void display_definitions( const basic_block_t * const ptr_block)
{
	//get the head of the quadruple linked list;
	//traverse through the linked list printing the definitions
	quadruple_t * q = ptr_block->head;
	while(q)
	{
		printf("%s ",q->def);
		q = q->link;
		if(q) printf("-->");
	}
	printf("\n");
}


void input_flow_matrix(int n ; int flow[n][n] , int n)
{
	FILE *fp = fopen("flow_matrix","r");
	
	int i,j;
	for(i = 0 ; i < n ; ++i)
	{
		for( j = 0 ; j < n ; ++j)
		{
			fscanf( fp , "%d" , &flow[i][j] );
		}
	}

	fclose(fp);
}


_Bool checkId(char arr[50]);

void input_live_on_exit(basic_block_t* ptr_basic_block)
{
	
	char *exit_var = (char*)malloc( sizeof(char)*200);
	gets(exit_var);

	char *tok =strtok(exit_var," ,");
	
	while(tok != NULL)
	{
			if(checkId(tok))
			{
				addElements(&ptr_basic_block->exit, tok);
			}
			tok=strtok(NULL, " ,");
	}
	free( exit_var );
}

void init_all_exits( basic_block_t blocks[] , int n)
{
	//get the flow matrix
	//	for all basic blocks that have an exit
	//		input the variables live on exit.

	int flow[n+2][n+2];	//to hold the flow matrix with the entry and exit information.

	input_flow_matrix( flow , n+2);

	int i;
	for( i = 0 ; i < n+2 ; ++i)
	{
		if( flow[i][n+1] )
		{
			//input the variables on exit
			printf("Enter the variables that are live on exit of block %d: \n",i-1);
			input_live_on_exit( &blocks[i-1] );
		}
	}
}
//********************************************************************************************************************************************


void set_to_minus_1( int n ; int mat_redef[n][n] , int n  , int index )
{
	int i = 0;
	for( i = 0 ; i < n ; ++i)
	{
		mat_redef[index][i] = -1;
	}
}


_Bool is_row_0( int n ; int mat[n][n] , int n , int row)
{
	_Bool zero_row = 1;
	int i;
	for(i = 0 ; i < n ; ++i)
	{
		zero_row = zero_row && !mat[row][i];
	}
	return zero_row;
}

int get_all_0_row( int n ; int mat_redef[n][n] , int n )
{
	int i;
	for( i = 0 ;  i < n ; ++i)
		if( is_row_0( mat_redef , n , i ) )
			return i;
	printf("There is no 0 row.....\n line : 275 , file : implement.c");
	exit(0);
}

void remove_edge_to_index(int n; int a[][n],int n, int index)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<n; ++j)
		{
			if(j==index)
			{
				if( a[i][j] != -1 )
				a[i][j]=0;
			}
		}
	}
}

_Bool is_propogation_finished(int n; int a[][n], int n)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<n; ++j)
		{
			if(a[i][j] != -1)
			{
				return 0;
			}
		}
	}
	return 1;
}

void get_redefindedMat( int n ; int mat_redef[n-2][n-2] , int flow_matrix[n][n] , int n)
{
	int i,j;
	for( i = 1 ; i < n-1 ; ++i)
	{
		for( j = 1 ; j < n-1 ; j++)
		{
			mat_redef[i-1][j-1] = flow_matrix[i][j];
		}
	}
}


void set_exit_for_particular_block(int n; basic_block_t blocks[] , int index, int mat_redef[][n], int n)
{
	int i;
	for(i=0; i<n; ++i)
	{
		if( mat_redef[index][i] )
		{
			blocks[index].exit = doUnion( &blocks[index].exit , &blocks[i].entry );
		}
	}
}



void propogate_liveness( basic_block_t * ptr_block );

void propogate_live_var_in_all( int n ; basic_block_t blocks[] , int flow_matrix[n][n] , int n )
{
	//remember that n is +2 than actual number of blocks
	int mat_redef[n-2][n-2];
	get_redefindedMat( mat_redef , flow_matrix , n);

	int temp[n-2][n-2];
	copy_matrix(temp,mat_redef,n-2);

	printf("CFG graph after removing the cycles and ENTRY EXIT block : \n");
	displayMatrix(n-2,mat_redef);

	//now we have everything to propogate
	//Till every block row is -1 ie propogated
	//	find the first all 0 row
	//		propogate it
	//		set everything in that row to -1
	//	whichever block it is pointed by make that edge 0

	while( !is_propogation_finished( mat_redef , n-2 ) )
	{
		int index = get_all_0_row( mat_redef , n-2 );
		//printf("%4d\n",index);
		
			//setting the exits of that particular block
			//find the union of all the variables in the entry set  of its connections...
		set_exit_for_particular_block(blocks, index, temp ,n-2);

		propogate_liveness( &blocks[index] );
		set_to_minus_1( mat_redef , n-2  , index );
		remove_edge_to_index( mat_redef , n-2 , index);
	}
}



//********************************************************************************************************************************************
//finds the usage of all the statements

_Bool checkId(char arr[50])
{
	int i=0;
	if (!(isalpha(arr[i])) || arr[i]=='_')	return 0;
	++i;
	while(arr[i] != '\0')
	{
		if(!(isalnum(arr[i]) || arr[i]=='_'))
		return 0;
		++i;
	}
	return 1;
}

void setUsage( quadruple_t * q )
{
	//for safety copy into another temporary variable.
	char instDummy[50];
	strcpy( instDummy , q->inst);

	char *tok1 = strtok( instDummy , " =-+");

        if(  findEqualsInString( q->inst ) )
	{
		char* tok2 = strtok( NULL , " =-+*/");
		if( checkId(tok2 ))
		{
			//add the usage to the quadruple set
			addElements(&q->live,tok2);
		}

		char *tok3 = strtok(NULL , "+-*/ ");
		if( tok3 && checkId(tok3) )
		{
			//add the usage to the quadruple set
			addElements( &q->live, tok3 );
		}
	}
	else if( isFirstTokenStore( tok1 ) )
	{
		char *tok2 = strtok( NULL , " -+");
		if( checkId(tok2) )
		{
			//add the usage to the quadruple set
			addElements( &q->live , tok2 );
		}
	}
}

void set_usages( basic_block_t *ptr_block )
{
	quadruple_t *ptr_quad = ptr_block->head;
	while( ptr_quad )
	{
		//printf("%s      : ",ptr_quad->inst);
		setUsage( ptr_quad );
		//display( &ptr_quad->live );
		ptr_quad = ptr_quad->link;
	}
}

void set_all_usages( basic_block_t blocks[] , int n )
{
	int i;
	for( i = 0 ; i < n ; ++i)
	{
		set_usages( &blocks[i] );
	}
}

void display_usage( const basic_block_t *const ptr_block)
{
	//get the head of the quadruple linked list;
	//traverse through the linked list printing the usage sets
	quadruple_t * q = ptr_block->head;
	while(q)
	{
		display( &q->live );
		q = q->link;
		if(q) printf("-->");
	}
	printf("\n");
}



//**********************************************work on 28/11/12**************************

int get_index(set_t* s, char* var)
{
	int index =-1;
	node_set_t* temp = s->head;

	while(temp )
	{
		index++;
		if(strcmp(temp->variable, var) == 0) break;

		temp= temp->link;
	}

	if(index<0)
	{
		
		printf("index less than zero\n");
		exit(0);
	}
	return index;
}

void set_graph_for_quad(int n; basic_block_t* block , quadruple_t* q,  int input[][n],int n, set_t* all_var_set)
{
	
	//for a particular quadruple
	//	get the set.

	set_t * ptr_set = &q->live;

	node_set_t * i = ptr_set->head;
	node_set_t *j = NULL;

	while(i)
	{
		j = i->link;
		while(j)
		{
			int index1 = get_index( all_var_set , i->variable);
			int index2  = get_index( all_var_set , j->variable );
			input[index1][index2] = input[index2][index1] = 1;
			j = j->link;
		}
		i = i->link;
	}
}

void getGraphMatrix_forEveryBlocks(int n;basic_block_t* block ,int input[][n],int n, set_t* all_var_set)
{

	
	quadruple_t* q= block->head;
	
	while(q)
	{
		set_graph_for_quad(block,q,  input, n,all_var_set);
		q = q->link;
	}
	
}

void getGraphMatrix(int n;int input[][n], basic_block_t blocks[], int n,  set_t* all_var_set , int num_of_blocks)
{
	int i=0;
	for(i=0;i<num_of_blocks;i++)
	{
		getGraphMatrix_forEveryBlocks(&blocks[i],input,n,all_var_set); 
	}
}



void init_matrix(int n; int input[][n], int n)
{
	int i,j;
	for(i=0; i<n; ++i)
	{
		for(j=0; j<n; ++j)
		{
			input[i][j]=0;
		}
	}
}


