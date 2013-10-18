#include<stdio.h>
#include<string.h>
#include<assert.h>
#include"CFG.h"

void init_block( basic_block_t *ptr_block , int id)
{
	ptr_block->head = 0;
	ptr_block->id = id;
	initSet( &ptr_block->entry );
	initSet( &ptr_block->exit );
}

void init_blocks( basic_block_t blocks[] , int n )
{
	int i;
	for( i = 0 ; i < n ; ++i)
	{
		init_block( &blocks[i] , i);
	}
}

char* get_next_line( FILE *fp)
{
	char *temp = (char*)malloc(sizeof(char)*50);
	char ch;
	int counter = 0;
	while( (ch = fgetc(fp)) != '\n' )
	{
		temp[counter++] = ch;
	}
	temp[counter] = '\0';
	return temp;
}


void get_input( basic_block_t blocks[] , int n)
{
	FILE *fp = fopen("input1","r");
	assert(fp!=0);

	char *line;

	int i = 0;
	while( i < n )
	{
		line = get_next_line( fp );
		//if line is ----------
		//  increament i
		//else
		//add at the end
		if( !strcmp( line , "----------"))
		{
			++i;
		}
		else
		{
			add_at_end( &blocks[i] , line );
		}
	}

	fclose(fp);
}

void disp_basic_block(const basic_block_t *ptr_basic_block )
{
	quadruple_t *temp = ptr_basic_block -> head;
        while(temp)
        {
            printf("%s", temp->inst);
            temp = temp->link;
            if(temp) printf("  --->  ");
        }
        printf("\n");
}


void disp_basic_blocks(const basic_block_t* blocks, int n)
{

    int i=0;
    for(i=0; i<n; ++i)
    {
	printf("block %d :",i);
	disp_basic_block( &blocks[i] );
    }

}

void write_matrix_into_file(int num_of_var; int displayMatrix[][num_of_var] , int num_of_var, char* filename)
{
	int i,j;
	FILE *fp;
	fp=fopen(filename,"w");
        for(i=0; i<num_of_var; i++)
        {
                for(j=0; j<num_of_var; j++)
                {
                        	fprintf(fp,"%d",displayMatrix[i][j]);
				fprintf(fp," ");
                }
		fprintf(fp,"\n");
        }
	fclose(fp);
}
