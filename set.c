#include<stdio.h>
#include"set.h"

void initSet(set_t* s)
{
	s->head=0;
}

char* getVarFromIndex(int index, set_t* s)
{
	
	node_set_t* trav = s->head;
	int i=0;
	for(i=0;i<index;++i)
	{
		
		trav= trav->link;
	}
	return trav->variable;
}

void addElements(set_t* s,char variable[])
{
	node_set_t* temp = (node_set_t*)malloc(sizeof(node_set_t));
	strcpy(	temp->variable, variable);
	temp->link = 0;
	int unique =1;
	
		//if head is null add it
	if(s->head == 0)
	{
		s->head = temp;
	}
		//else traverse till the end and keep checking whether it is unique or not, if unique add it.
	else
	{
		node_set_t* pres = s->head;
		node_set_t* prev = 0;
		while(pres && unique)
		{
			if(strcmp(pres->variable,temp->variable)==0)
			{
				unique = 0;
			}
			prev = pres;
			pres = pres->link;	//for moving further.
		}
	

		if(unique && pres==0)
		{
			prev->link = temp;
			temp->link =0;
		}
	}
}



set_t doUnion(const set_t* s1, const set_t* s2)
{
	

	set_t unionSet;
	
	initSet( &unionSet );
	
	node_set_t* traverse;
	if(s1)
	{
		traverse = s1->head;
		
		while(traverse)
		{
			addElements(&unionSet, traverse->variable);
			traverse = traverse->link;
		}
	}
	if(s2)
	{
		traverse = s2->head;
		while(traverse)
		{
			addElements(&unionSet, traverse->variable);
			traverse = traverse->link;
		}
	}
	return unionSet;
}

set_t copy(set_t *s)
{
	set_t temp = doUnion( NULL , s);
	return temp;
}



void display(const set_t* s)
{
	printf("{");
	
	node_set_t* temp = s->head;
	while(temp)
	{
		printf("%s", temp->variable);
		temp = temp->link;
		if(temp) printf(",");
	}	
	printf("}");

}


int get_num_of_elements( const set_t * const ptr)
{
	node_set_t * temp = ptr -> head;
	int count = 0;

	while( temp )
	{
		temp = temp -> link;
		++count;
	}
	return count;
}

void removeFromSet(set_t* s, char* str)
{
	if(s && s->head!=NULL)
	{
		node_set_t* temp = s->head;
	
		node_set_t* prev = 0;

		while(temp && strcmp(temp->variable, str)!=0)
		{
			prev= temp;	
			temp = temp->link;
		} 	
	
		if(temp == s->head)
		{
			s->head = temp->link;
			free(temp);
			
		}
		else if(temp != NULL)
		{
			prev->link = temp->link;
			free(temp);		
		}
	}
}

void deallocate(set_t* s)
{
	if(s && s->head!= NULL)
	{
		node_set_t* temp = s->head;
			
		while(temp)
		{
			s->head = temp->link;
			free(temp);		
		}
	}	
}

		

