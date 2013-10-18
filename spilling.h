#include"set.h"
#include"CFG.h"

#ifndef SPILL
#define SPILL SPILL

char* getLoadInstruction( char* var );
char* getStoreInstruction( char * var);
void freeInstruction( char* inst);
char* get_var_for_spilling(int num_of_var; int graph[][num_of_var] , int num_of_var , set_t* all_var_set );
int get_max_degree(int n; int mat[n][n], int n);
void spillVariable( basic_block_t blocks[] , char* var , int num_blocks );

#endif
