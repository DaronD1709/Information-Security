#include <stdio.h>
#include <string.h>
void redundant_code(char* p)
{
    char local[256];
    strncpy(local,p,20);
	printf("redundant code\n");
}
int main(int argc, char* argv[])
{
	char buffer[16];
	strcpy(buffer,argv[1]);
	return 0;
}