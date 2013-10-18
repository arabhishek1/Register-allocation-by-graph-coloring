#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{

	char a[15];
	char b[15];
	strcpy(b, "asdf");
	strcpy(a,b);
	printf("\n%s",a);
	printf("\n%s",b);
	return 0;
	
}
