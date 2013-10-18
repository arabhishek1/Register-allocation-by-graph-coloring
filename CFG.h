#include<stdlib.h>
#include"set.h"

#ifndef BASIC_BLOCK
#define BASIC_BLOCK

struct quadruple
{
	char inst[50];
	char def[15];
	set_t live;
	struct quadruple *link;	
};
typedef struct quadruple quadruple_t;

//The structure of the basic block to be followed.
struct basic_block
{
	int id;			//keeping the basic block number
	quadruple_t * head;	//start of linked list for a quadraple list.
				//each node contains a quadraple
	set_t entry;
	set_t exit;
};

typedef struct basic_block basic_block_t;


//initialization functions
void init_blocks( basic_block_t blocks[] , int n );

void add_at_end(basic_block_t* ptr_basic_block, char str[50]);

void disp_basic_blocks(const basic_block_t* ptr_basic_block,int n);

void get_input( basic_block_t blocks[] , int n);

void reverse_basic_block( basic_block_t *ptr_basic_block);

void disp_basic_block(const basic_block_t *ptr_basic_block );

void init_all_exits( basic_block_t blocks[] , int n);

void input_flow_matrix(int n ; int flow[n][n] , int n);



//our algorithm functions
void set_all_definitions( basic_block_t blocks[] , int n );

void set_all_usages( basic_block_t blocks[] , int n );

void set_usages( basic_block_t *ptr_block );

void propogate_liveness( basic_block_t * ptr_block );

void remove_cycles( int n ; int mat[n][n] , int n , basic_block_t blocks[] );

void input_live_on_exit(basic_block_t* ptr_basic_block);

void propogate_live_var_in_all( int n ; basic_block_t blocks[] , int flow_matrix[n][n] , int n );

int get_num_of_elements(set_t *all_var_set);

void set_all_var_CFG( basic_block_t *blocks , set_t* all_var_set , int n );


//************

void getGraphMatrix(int n;int input[][n], basic_block_t blocks[], int n,  set_t* all_var_set , int num_of_blocks);
#endif
