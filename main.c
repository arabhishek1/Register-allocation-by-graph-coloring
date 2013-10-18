#include"CFG.h"
#include "impl.h"
#include "set.h"
#include "spilling.h"

#define DEBUG 0

#if DEBUG
char* find_definition( char* str );
_Bool isFirstTokenLoad( char *str );
void disp_basic_block(const basic_block_t *ptr_basic_block );
void display_definitions( const basic_block_t * const ptr_block);
void display_usage( const basic_block_t *const ptr_block);
#endif
void remove_cycles( int n ; int mat[n][n] , int n , basic_block_t blocks[] );

int main()
{
	//we have to input the from the file
	int n_b = 4;
	basic_block_t blocks[n_b];
	init_blocks( blocks , n_b );
	get_input( blocks , n_b);	//Takes the input for all basic blocks from the input file

	init_all_exits( blocks , n_b);			//	for all basic blocks that have an exit
							//		input the variables live on exit.

	int flow_matrix[n_b+2][n_b +2];
	input_flow_matrix( flow_matrix , n_b+2);

	remove_cycles( flow_matrix , n_b +2 , blocks);

	set_t all_var_set;				//This set is used to store all the different variables in the control flow graph
	


// critical section begins ****************************************
	_Bool sufficient = 0;

	do{

	/*-->*/	set_all_definitions( blocks , n_b );	//sets all definitions in the file i.e a = b + c : a is defined & load a: a is defined
	/*-->*/	set_all_usages( blocks , n_b);		//set all used variables in a intructions i.e a = b + c : b and c are usages.
			initSet( &all_var_set );			//Initializing this set


	/*-->*/	propogate_live_var_in_all( blocks , flow_matrix , n_b+2 );

		int i;
		for(i = 0 ; i < n_b ; ++i )
		{
			printf("blocks[%d].exit : " , i);
			display( &blocks[i].exit ); printf("\n");
			printf("blocks[%d].entry : " , i);
			display( & blocks[i].entry ); printf("\n\n");
		}

		set_all_var_CFG( blocks , &all_var_set , n_b );

		int num_of_var = get_num_of_elements(&all_var_set);

		int regInterferenceGraph[num_of_var][num_of_var];			//graph is the Register interference graph.

		init_matrix(regInterferenceGraph,num_of_var);

		getGraphMatrix(regInterferenceGraph , blocks, num_of_var,  &all_var_set , n_b);

		printf("\n\nThe set of all variables in the Control flow graph : ");	
		display(&all_var_set); printf("\n");

		printf("\n the matrix for interference of variables is:\n");
		displayMatrix(num_of_var, regInterferenceGraph );


		write_matrix_into_file( regInterferenceGraph , num_of_var, "RIG.txt");

		sufficient = assign_colours(num_of_var, "RIG.txt", &all_var_set);

		if(!sufficient)
		{
			
			printf("Now we are going to spill using spilling logic\n");

			char * temp = get_var_for_spilling( regInterferenceGraph , num_of_var , &all_var_set );
			char var[50];
			strcpy( var , temp );
			printf("Variable going to be spilled : %s",var);

			spillVariable(  blocks , var , n_b );
		}

	}while( !sufficient );
// critical section ends ****************************************
	
#if DEBUG

	//adding c , d , e , f on exit of the exit set of block 0
	addElements( &blocks[2].exit , "c");
	addElements( &blocks[2].exit , "b");
	addElements( &blocks[2].exit , "f");

	printf("Live on exit : ");
	display( &blocks[2].exit );
	printf("\n");

	printf("block : ");
	disp_basic_block( &blocks[2] );

	printf("def   : ");
	display_definitions( &blocks[2] );

	printf("usage : ");
	display_usage( &blocks[2] );

	propogate_liveness( &blocks[2] );

	printf("\nAfter propogation....\nusage : ");
	display_usage( &blocks[2] );

	printf("The live on entry set : ");
	display( &blocks[2].entry );
	printf("\n");
	

#endif

	return 0;
}

/*
	while there are no sufficient registers
		propogate all the live variables
		print what ever you want
		get all the variables in the control flow graph
		make register interference graph
		if there are insufficient registers
			get the max degree var
			spill the variable
				remove all its usages and definitions
				for every occurence of the variable
				add the store or load instruction
*/
