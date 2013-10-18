#include"CFG.h"


char* getNextElem(set_t* ptr_set)
{
	static node_set_t* elemPointer=0;
	if(ptr_set)
	{
		elemPointer = ptr_set->head;
	}
	
	node_set_t* temp=elemPointer;
	if(elemPointer)
	{
		elemPointer=elemPointer->link;
	}
	if(temp)
	return temp->variable;
	return 0;
	
}

void propogate_variable( quadruple_t *q , char var[] )
{
	while( q )
	{
		//if the def is in the quadruple then stop
		//if not then
		//	add the element in the quadruple live set
		//	move to the next quadruple

		if( strcmp( var , q->def ) == 0 )
			while(q) q = q->link;
		else
		{
			addElements( &q->live , var );
			q = q->link;
		}
	}
}


//if problem occurs then this may have a problem
void propogate_for_quad( quadruple_t *ptr_quad )
{
	if( ptr_quad )
	{
		char* var_ptr = getNextElem( &ptr_quad->live );
	
		if( var_ptr )
			propogate_variable( ptr_quad->link , var_ptr );

		while( ( var_ptr = getNextElem( NULL )) )
		{
			propogate_variable( ptr_quad->link , var_ptr );
		}
	}
}


void propogate_live_on_exit( basic_block_t *ptr_block )
{
	if( ptr_block )
	{
		char* var_ptr = getNextElem( &ptr_block -> exit);

		if( var_ptr )
			propogate_variable( ptr_block -> head , var_ptr );

		while( (var_ptr = getNextElem( NULL )) )//the next element in the set
		{
			propogate_variable( ptr_block -> head , var_ptr );
		}
	}
}

void propogate_for_every_quad( basic_block_t *ptr_block )
{
	//get the head of the quadruple
	//	do for every quadruple
	quadruple_t * ptr_quad = ptr_block -> head;
	while( ptr_quad )
	{
		//propogate function for one quadruple
		propogate_for_quad( ptr_quad );
		ptr_quad = ptr_quad->link;
	}
}

void make_entry_set( basic_block_t *ptr_block )
{
	if( ptr_block && ptr_block -> head)
	{
		ptr_block -> entry = copy( & ptr_block -> head -> live );
	}
}


void propogate_liveness( basic_block_t * ptr_block )
{
	//pre requisite gain a basic block

	//reverse the basic block
	reverse_basic_block( ptr_block );

	//propogate all variables that are live on exit
	propogate_live_on_exit( ptr_block );
	//for every quadruple propogate their usages.
	propogate_for_every_quad( ptr_block );

	//reverse the basic block
	reverse_basic_block( ptr_block );

	//after this : the set of the first instruction is : live on entry set.
	make_entry_set( ptr_block );
}

//***********************************************************************************************************************
//cyclicity finding functions

void travel( int n ; int mat[n][n] , int n , int visited[] , int place , basic_block_t blocks[] );

void find_cycle( int n; int mat[n][n], int n , basic_block_t blocks[]);

void remove_cycles( int n ; int mat[n][n] , int n , basic_block_t blocks[] )
{
	find_cycle( mat, n , blocks);
}

void find_cycle( int n; int mat[n][n], int n , basic_block_t blocks[] )
{

	//we need to find at every point in time if there is cycle formed.

	//for every node we have to be optimistic to find a cycle

	//making a visited vector and initialising it
	int visited[n];

	int i = 0;

	for( i = 0 ; i < n ; ++i ) visited[i] = 0;

	//try to find a cycle for every node

	for( i = 0 ; i<n ; ++i)
	{
		visited[i] = 1;

		travel( mat , n , visited , i , blocks);

		visited[i] = 0;
	}

}

void travel( int n ; int mat[n][n] , int n , int visited[] , int place , basic_block_t blocks[] )
{


	//for every next adjacent node check if you have found the required cycle
	int i;
	for( i = 0 ; i < n ; ++i)
	{
		//if there is a way to this node..
		//	and this has already been seen then this is a cycle
		if( mat[place][i] )
		{
			if( !visited[i] )
			{
				//go in front and explore
				visited[i] = 1;

					//call the function recursively
					travel( mat , n , visited , i , blocks );

				visited[i] = 0;
			}
			else
			{
				printf("There is a cycle one of whose edje is : %d -> %d\nWe remove this edje..\n", place-1 , i-1 );
				printf("Please enter the variables that are live on exit from block %d -> block %d : ",place-1 , i-1);
				input_live_on_exit( &blocks[place-1] );
				//removing this cycle
				mat[place][i] = 0;
			}
		}
	}

}




//for the live set
void  set_all_var_liveSet( set_t *ptr_set , set_t *live )
{
	if( live )
	{
		char *temp = getNextElem( live );
		if( temp )
		{
			addElements( ptr_set , temp );
		}
		while( (temp = getNextElem( NULL )) )
		{
			addElements( ptr_set , temp );
		}
	}
}

//for every quad
void set_all_var_quad( quadruple_t *p_quad , set_t *ptr_set )
{
	if( p_quad )
	{
		set_all_var_liveSet(  ptr_set , &p_quad -> live  );
	}

}

//for each block
void set_all_var_block( basic_block_t *ptr_block , set_t *ptr_set )
{
	if( ptr_block )
	{
		//for every quadruple in the block
		quadruple_t* p_quad = ptr_block->head;

		while( p_quad )
		{
			set_all_var_quad( p_quad , ptr_set );
			p_quad = p_quad ->link;
		}
	}
}

//for all the blocks
void set_all_var_CFG( basic_block_t blocks[] , set_t* ptr_set , int n )
{
	//for every variable
	int i;
	for( i = 0 ; i < n ; ++i)
	{
		//set_all_var_block
		set_all_var_block( &blocks[i] , ptr_set );
	}
}

